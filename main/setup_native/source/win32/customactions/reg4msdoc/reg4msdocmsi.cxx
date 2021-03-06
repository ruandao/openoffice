/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifdef _MSC_VER
#pragma warning(push, 1) /* disable warnings within system headers */
#endif
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>
#include <msiquery.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <tchar.h>
#include "register.hxx"
#include "msihelper.hxx"

#include <memory>
#include <string>

#define ELEMENTS_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

void DetermineWordPreselectionState(MSIHANDLE handle)
{
    if (query_preselect_registration_for_ms_application(handle, MSWORD))
        SetMsiProp(handle, TEXT("SELECT_WORD"));
}

void DetermineExcelPreselectionState(MSIHANDLE handle)
{
    if (query_preselect_registration_for_ms_application(handle, MSEXCEL))
        SetMsiProp(handle, TEXT("SELECT_EXCEL"));
}

void DeterminePowerPointPreselectionState(MSIHANDLE handle)
{
    if (query_preselect_registration_for_ms_application(handle, MSPOWERPOINT))
        SetMsiProp(handle, TEXT("SELECT_POWERPOINT"));
}

extern "C" UINT __stdcall InstallUiSequenceEntry(MSIHANDLE handle)
{        
    //MessageBox(NULL, TEXT("InstallUiSequenceEntry"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
    
    if (IsModuleSelectedForInstallation(handle, TEXT("gm_p_Wrt_Bin")))
    {
        DetermineWordPreselectionState(handle);
    }
    else if (IsModuleInstalled(handle, TEXT("gm_p_Wrt_Bin")) && 
             !IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Wrt_Bin")) && 
             IsRegisteredFor(handle, MSWORD))
    {             
        SetMsiProp(handle, TEXT("SELECT_WORD"));
    }        
    else
    {
        UnsetMsiProp(handle, TEXT("SELECT_WORD"));
    }
        
    if (IsModuleSelectedForInstallation(handle, TEXT("gm_p_Calc_Bin")))                    
    {
        DetermineExcelPreselectionState(handle);    
    }        
    else if (IsModuleInstalled(handle, TEXT("gm_p_Calc_Bin")) && 
             !IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Calc_Bin")) &&
             IsRegisteredFor(handle, MSEXCEL))
    {
        SetMsiProp(handle, TEXT("SELECT_EXCEL"));
    }
    else
    {
        UnsetMsiProp(handle, TEXT("SELECT_EXCEL"));
    }
        
    if (IsModuleSelectedForInstallation(handle, TEXT("gm_p_Impress_Bin")))         
    {
        DeterminePowerPointPreselectionState(handle);
    }       
    else if (IsModuleInstalled(handle, TEXT("gm_p_Impress_Bin")) && 
            !IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Impress_Bin")) &&
            IsRegisteredFor(handle, MSPOWERPOINT))
    {
        SetMsiProp(handle, TEXT("SELECT_POWERPOINT"));        
    }        
    else
    {
        UnsetMsiProp(handle, TEXT("SELECT_POWERPOINT"));
    }    

    SetMsiProp(handle, TEXT("UI_SEQUENCE_EXECUTED"));

	return ERROR_SUCCESS;
}

extern "C" UINT __stdcall InstallExecSequenceEntry(MSIHANDLE handle)
{
    //MessageBox(NULL, TEXT("InstallExecSequenceEntry"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
     
    // Do nothing in repair mode.
    // Then UI_SEQUENCE_EXECUTED is not set and Installed is set!
    // In silent installation UI_SEQUENCE_EXECUTED is also not set, but Installed is not set.
    if ((!IsSetMsiProp(handle, TEXT("UI_SEQUENCE_EXECUTED"))) && (IsMsiPropNotEmpty(handle, TEXT("Installed")))) { return ERROR_SUCCESS; }
    
    int reg4 = 0;
    int unreg4 = 0;
    
    // we always register as html editor for Internet Explorer 
    // if writer is installed because there's no harm if we do so 
	if (IsModuleSelectedForInstallation(handle, TEXT("gm_p_Wrt_Bin")))
		reg4 |= HTML_EDITOR;

    if (IsSetMsiProp(handle, TEXT("SELECT_WORD")) && !IsRegisteredFor(handle, MSWORD))
        reg4 |= MSWORD;
    else if (!IsSetMsiProp(handle, TEXT("SELECT_WORD")) && IsRegisteredFor(handle, MSWORD))
        unreg4 |= MSWORD;
        
    if (IsSetMsiProp(handle, TEXT("SELECT_EXCEL")) && !IsRegisteredFor(handle, MSEXCEL))
        reg4 |= MSEXCEL;
    else if (!IsSetMsiProp(handle, TEXT("SELECT_EXCEL")) && IsRegisteredFor(handle, MSEXCEL))
        unreg4 |= MSEXCEL;
        
    if (IsSetMsiProp(handle, TEXT("SELECT_POWERPOINT")) && !IsRegisteredFor(handle, MSPOWERPOINT))
        reg4 |= MSPOWERPOINT;
    else if (!IsSetMsiProp(handle, TEXT("SELECT_POWERPOINT")) && IsRegisteredFor(handle, MSPOWERPOINT))
        unreg4 |= MSPOWERPOINT;
                       	    
    if (reg4)
    {                       	    
	    Register4MsDoc(handle, reg4);
    }
    
    if (unreg4)
    {        
        Unregister4MsDoc(handle, unreg4);
    }        
    return ERROR_SUCCESS;
}

extern "C" UINT __stdcall DeinstallExecSequenceEntry(MSIHANDLE handle)
{         
    //MessageBox(NULL, TEXT("DeinstallExecSequenceEntry"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
        	            
    if (IsCompleteDeinstallation(handle))
    {
        Unregister4MsDocAll(handle);  
        return ERROR_SUCCESS;
    }
    
    if (IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Wrt_Bin")))
    {
        Unregister4MsDoc(handle, MSWORD | HTML_EDITOR);
    }
    
    if (IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Calc_Bin")))
    {
        Unregister4MsDoc(handle, MSEXCEL);
    }
    
    if (IsModuleSelectedForDeinstallation(handle, TEXT("gm_p_Impress_Bin")))
    {
        Unregister4MsDoc(handle, MSPOWERPOINT);
    }
                            
    return ERROR_SUCCESS;
}
