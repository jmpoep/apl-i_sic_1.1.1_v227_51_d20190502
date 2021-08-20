"""
    @package GenSmipHeader
    usage: gen_header.py [-h] -f f -o o

    Use this script to generate header files based on FIT's XML layout file.

    optional arguments:
      -h, --help  show this help message and exit
      -f f        Input XML file
      -o o        Output H file

    Use this script to generate header files based on FIT's XML layout configuration.
    
    TODOS:
    1. Add support for bitfields
    2. Support arrays. The following: 
    
        typedef struct _IAFWSOCSkuInfo_
        {
            BoardINFO Board[0];
            BoardINFO Board[1];
            BoardINFO Board[2];
            BoardINFO Board[3];
            BoardINFO Board[4];

        } IAFWSOCSkuInfo;
        
       Should be converted to:
       
       typedef struct _IAFWSOCSkuInfo_
        {
            BoardINFO Board[5];
        } IAFWSOCSkuInfo;
        
    3. XML update: only reserved fields should use round_to
    4. Update copyright and #define guards
"""
import sys
import argparse
import os
import xml.etree.ElementTree as XmlParser
sys.path.append(os.path.normpath(os.path.dirname(os.path.realpath(__file__)) + '/../BuildScripts'))
import BuildHelperFunctions

"""
Revision History
================

Check header above for list of opens/issues.
Version 0.1:
    - Initial release
Version 0.2:
    - Added XML namespace query support. 
"""
VERSION = "0.2"

# XML Namespace.
NS=r'{http://www.intel.com/FtoolLayout}'

# Tab characters (4-spaces).
TAB_CHARS = "    "

# FTOOL types to header types conversion table.
NATIVE_TYPES_CONVERSION_TABLE = {
'byte':'UINT8',
'word':'UINT16',
'dword':'UINT32',
'bitfield32':'UINT32'
}

# Header file start template
HEADER_START = """
/*++

INTEL CONFIDENTIAL
Copyright {%Year%} Intel Corporation All Rights Reserved.

The source code contained or described herein and all documents
related to the source code ("Material") are owned by Intel Corporation
or its suppliers or licensors. Title to the Material remains with
Intel Corporation or its suppliers and licensors. The Material
contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by
worldwide copyright and trade secret laws and treaty provisions. No
part of the Material may be used, copied, reproduced, modified,
published, uploaded, posted, transmitted, distributed, or disclosed in
any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other
intellectual property right is granted to or conferred upon you by
disclosure or delivery of the Materials, either expressly, by
implication, inducement, estoppel or otherwise. Any license under such
intellectual property rights must be express and approved by Intel in
writing.

File Name:
    {%FileName%}
    
{%AutogenNotice%}
    
--*/

#ifndef _IAFW_SMIP_H_
#define _IAFW_SMIP_H_


#pragma pack(push,1)

"""

# Header file end template
HEADER_END = """
#pragma pack(pop)
#endif _IAFW_SMIP_H_
"""

class CStruct:
    """
        Class used to represet C structure. May want to convert this to ctypes later on.
        ctypes would be nice since it would give us a free C Python wrapper for the SMIP
        data.
    """
    def __init__(self, name, alias):
        self.name = name
        self.alias = alias
        self.data = [] # Data should be entered in-order
    
    def addDataEntry(self, name, type, numEntries, emptyArray):
        """
            Add variable to data structure.
            name: Name of the variable.
            type: Variable type (see NATIVE_TYPES_CONVERSION_TABLE keys).
            numEntries: multiplicity of the type (more than 1 for array type).
        """
        entry = {'name':name, 'type':type, 'numEntries':numEntries, 'emptyArray':emptyArray}
        self.data.append(entry)
        
    def toCStructure(self):
        """
            Returns instance class in 'C' format.
        """
        if len(self.data) == 0:
            return ""
        
        txt = ""
        
        name = self.name
        if self.alias is not None:
            txt += """\n// XML original class name: """ + name
            name = self.alias
            
        txt += """
typedef struct _""" + name + """_
{
"""+ self.toCVars() +"""
} """ + name + """;
        """
        return txt
        
    def toCVars(self):
        """
            Retuns data entries in 'C' format
        """
        txt = ""
        for var in self.data:
            txt += TAB_CHARS + var['type'] + " "
            
            # Empty arrays are intepreted as pointers. This is probably not ideal, 
            # but we have to workaround the MSVC compiler warning C4200.
            if var['emptyArray'] is True:
                txt += '*'
            txt +=var['name']
            if var['numEntries'] > 1:
                txt += "[" + str(var['numEntries']) + "]"
            txt += ";\n"
        return txt
        

class GenHeader:
    """
        Loads FTOOL XML layout file and converts all defined classes
        into C structures.
    """
    def __init__(self, inputPath):
        self.inputPath = inputPath
        self.structs = [] # Set as array for now.
        self.root = XmlParser.parse(self.inputPath)
    
    def parseXml(self):
        """
            Parse XML layout file.
        """
        classes = self.root.findall('./{0}Class'.format(NS))
        print "Number of class definitions: {0}".format(len(classes))
        for classDef in classes:
            self.parseClass(classDef)
            
    def parseClass(self, classDef):
        """
            Parse single XML Class definition.
        """
        className = classDef.get('name')
        classAlias = classDef.get('c-class-alias')

        layoutEntries = classDef.findall('./{0}LayoutEntry'.format(NS))
        if len(layoutEntries) == 0:
            return
        cStruct = CStruct(className, classAlias)
        for layoutEntry in layoutEntries:
            self.parseLayoutEntry(layoutEntry, cStruct)
        self.structs.append(cStruct)
    
    def parseLayoutEntry(self, layoutEntry, cStruct):
        """
            Parse single XML LayoutEntry tag.
        """
        type = layoutEntry.get("type")
        name = layoutEntry.get("name")
        bytesCount = 1
        skipClass = False
        emptyArray = False
        
        skipBlob = layoutEntry.get("skip-c-gen")
        if skipBlob is not None and skipBlob == "true":
            return
        
        # For now, only support round_to for Reserved class.
        if type == 'class:Reserved':
            roundTo = layoutEntry.get("round_to")
            bytesCount = int(roundTo)
            type = NATIVE_TYPES_CONVERSION_TABLE['byte']
        elif "class:" in type:
            # Do this in a separate step to avoid the native type
            # conversion.
            type = type.replace("class:","")
            if type == "BLOB":
                # Require a class type alias defined otherwise return an error.
                typeAlias = layoutEntry.get("c-array-type")
                if typeAlias is None or len(typeAlias) == 0:
                    errorMessage = """type class:BLOB should have skip-c-gen set to true or c-array-type defining an alias array name"""
                    sys.exit(errorMessage)
                else:
                    # Only empty arrays are allowed at this time.
                    type = typeAlias
                    emptyArray = True
        else:
            # this will throw an exception if the native type
            # is not defined in the NATIVE_TYPES_CONVERSION_TABLE
            type = NATIVE_TYPES_CONVERSION_TABLE[type]
            
        if skipClass is not True:
            cStruct.addDataEntry(name, type, bytesCount, emptyArray)
        
    def getText(self):
        """
            Generate header file containing collection of XML classes converted
            into C structs.
        """
        txt = HEADER_START
        for cStruct in self.structs:
            txt += cStruct.toCStructure()
        txt += HEADER_END
        return txt

    
def cliParser():
    """
        Command line parser for gen_header script.
    """
    parser = argparse.ArgumentParser(description="H header generation script", \
    epilog="Use this script to generate header files based on FIT's XML layout file.")
    parser.add_argument('-f', metavar='f', type=str, help='Input XML file.', required=True)
    parser.add_argument('-o', metavar='o', type=str, help='Output H file',   required=False)
    args = parser.parse_known_args()
    args = args[0] # only look at known args
    return args


def Run(InXml, OutHeader):

    # Normalize directory variables.
    InXml = BuildHelperFunctions.OsPath(InXml)
    OutHeader = BuildHelperFunctions.OsPath(OutHeader)

    headerGen = GenHeader(InXml)
    headerGen.parseXml()
    headerTxt = headerGen.getText()

    with open(OutHeader, 'w') as f:
        f.write(headerTxt)


if __name__ == "__main__":
    cli = cliParser()
    Run(cli.f, cli.o)