'encoding UTF-8  Do not remove or change this line!
'*************************************************************************
'
'  Licensed to the Apache Software Foundation (ASF) under one
'  or more contributor license agreements.  See the NOTICE file
'  distributed with this work for additional information
'  regarding copyright ownership.  The ASF licenses this file
'  to you under the Apache License, Version 2.0 (the
'  "License"); you may not use this file except in compliance
'  with the License.  You may obtain a copy of the License at
'  
'    http://www.apache.org/licenses/LICENSE-2.0
'  
'  Unless required by applicable law or agreed to in writing,
'  software distributed under the License is distributed on an
'  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
'  KIND, either express or implied.  See the License for the
'  specific language governing permissions and limitations
'  under the License.
'
'*************************************************************************
'*
'*  short description : Form controls in BASIC-IDE dialogs
'*
'\******************************************************************************

sub main

    use "framework\optional\includes\basic_formcontrols.inc"

    call hStatusIn( "framework" , "f_basic_formcontrols.bas" )
    
    hSetMacroSecurityAPI( GC_MACRO_SECURITY_LEVEL_LOW )
    
    printlog( "OpenOffice.org 2.x" )
    call tAllControlsOnDialog( "current" )  ' OpenOffice.org 2.X
    
    hSetMacroSecurityAPI( GC_MACRO_SECURITY_LEVEL_DEFAULT )
    
    call hStatusOut()

end sub

'-------------------------------------------------------------------------

sub LoadIncludeFiles

    use "global\system\includes\master.inc"
    use "global\system\includes\gvariabl.inc"
    
    use "global\tools\includes\optional\t_treelist_tools.inc"    
    use "global\tools\includes\optional\t_basic_ide_tools.inc"
    use "global\tools\includes\optional\t_security_tools.inc"
    use "global\tools\includes\optional\t_docfuncs.inc"
    use "global\tools\includes\optional\t_filetools.inc"
    use "global\tools\includes\optional\t_basic_organizer_tools.inc"

    use "framework\tools\includes\pbrowser_tools.inc"
    use "framework\tools\includes\formcontrols.inc"
    
    gApplication = "WRITER"
    call GetUseFiles()

end sub

'-------------------------------------------------------------------------
