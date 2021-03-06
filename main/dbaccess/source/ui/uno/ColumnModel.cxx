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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_dbui.hxx"

#ifndef DBAUI_COLUMNMODEL_HXX
#include "ColumnModel.hxx"
#endif
#ifndef _COM_SUN_STAR_AWT_FONTRELIEF_HPP_
#include <com/sun/star/awt/FontRelief.hpp>
#endif
#ifndef _COM_SUN_STAR_AWT_FONTEMPHASISMARK_HPP_
#include <com/sun/star/awt/FontEmphasisMark.hpp>
#endif
#ifndef _COM_SUN_STAR_BEANS_PROPERTYATTRIBUTE_HPP_
#include <com/sun/star/beans/PropertyAttribute.hpp>
#endif

#ifndef _CPPUHELPER_QUERYINTERFACE_HXX_
#include <cppuhelper/queryinterface.hxx>
#endif
#ifndef _COMPHELPER_EXTRACT_HXX_
#include <comphelper/extract.hxx>
#endif
#ifndef DBACCESS_SHARED_DBUSTRINGS_HRC
#include "dbustrings.hrc"
#endif
#ifndef _DBU_REGHELPER_HXX_
#include "dbu_reghelper.hxx"
#endif
#ifndef _TOOLKIT_UNOHLP_HXX
#include <toolkit/helper/vclunohelper.hxx>
#endif
#ifndef _COMPHELPER_PROPERTY_HXX_
#include <comphelper/property.hxx>
#endif

extern "C" void SAL_CALL createRegistryInfo_OColumnControlModel()
{
	static ::dbaui::OMultiInstanceAutoRegistration< ::dbaui::OColumnControlModel> aAutoRegistration;
}

//.........................................................................
namespace dbaui
{
//.........................................................................
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::container;
using namespace ::com::sun::star::awt;
using namespace ::com::sun::star::io;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::util;


DBG_NAME(OColumnControlModel)
//------------------------------------------------------------------
OColumnControlModel::OColumnControlModel(const Reference<XMultiServiceFactory>& _rxFactory)
    :OPropertyContainer(m_aBHelper)
    ,OColumnControlModel_BASE(m_aMutex)
    ,m_xORB(_rxFactory)
    ,m_sDefaultControl(SERVICE_CONTROLDEFAULT)
    ,m_bEnable(sal_True)
    ,m_nBorder(0)
    ,m_nWidth(50)
{
	DBG_CTOR(OColumnControlModel,NULL);
	registerProperties();	
}
// -----------------------------------------------------------------------------
OColumnControlModel::OColumnControlModel(const OColumnControlModel* _pSource,const Reference<XMultiServiceFactory>& _rxFactory)
    :OPropertyContainer(m_aBHelper)
    ,OColumnControlModel_BASE(m_aMutex)
    ,m_xORB(_rxFactory)
    ,m_sDefaultControl(_pSource->m_sDefaultControl)
    ,m_aTabStop(_pSource->m_aTabStop)
    ,m_bEnable(_pSource->m_bEnable)
    ,m_nBorder(_pSource->m_nBorder)
    ,m_nWidth(50)
{
	DBG_CTOR(OColumnControlModel,NULL);
	registerProperties();
}
// -----------------------------------------------------------------------------
OColumnControlModel::~OColumnControlModel()
{
	DBG_DTOR(OColumnControlModel,NULL);
	if ( !OColumnControlModel_BASE::rBHelper.bDisposed && !OColumnControlModel_BASE::rBHelper.bInDispose )
	{
		acquire();
		dispose();
	}	
}
// -----------------------------------------------------------------------------
void OColumnControlModel::registerProperties()
{
	registerProperty( PROPERTY_ACTIVE_CONNECTION, PROPERTY_ID_ACTIVE_CONNECTION, PropertyAttribute::TRANSIENT | PropertyAttribute::BOUND,
		&m_xConnection, ::getCppuType( &m_xConnection ) );
	Any a;
	a <<= m_xColumn;
//	registerMayBeVoidProperty( PROPERTY_COLUMN, PROPERTY_ID_COLUMN, PropertyAttribute::TRANSIENT | PropertyAttribute::BOUND| PropertyAttribute::MAYBEVOID,
//			&a, ::getCppuType( &m_xColumn ) );
	registerProperty( PROPERTY_COLUMN, PROPERTY_ID_COLUMN, PropertyAttribute::TRANSIENT | PropertyAttribute::BOUND,
			&m_xColumn, ::getCppuType( &m_xColumn ) );

	registerMayBeVoidProperty( PROPERTY_TABSTOP, PROPERTY_ID_TABSTOP, PropertyAttribute::BOUND | PropertyAttribute::MAYBEVOID,
			&m_aTabStop, ::getCppuType( static_cast<sal_Int16*>(NULL) ) );
	registerProperty( PROPERTY_DEFAULTCONTROL, PROPERTY_ID_DEFAULTCONTROL, PropertyAttribute::BOUND,
			&m_sDefaultControl, ::getCppuType( &m_sDefaultControl ) );
	registerProperty( PROPERTY_ENABLED, PROPERTY_ID_ENABLED, PropertyAttribute::BOUND,
			&m_bEnable, ::getCppuType( &m_bEnable ) );
	registerProperty( PROPERTY_BORDER, PROPERTY_ID_BORDER, PropertyAttribute::BOUND,
		&m_nBorder, ::getCppuType( &m_nBorder ) );
	registerProperty( PROPERTY_EDIT_WIDTH, PROPERTY_ID_EDIT_WIDTH, PropertyAttribute::BOUND,
		&m_nWidth, ::getCppuType( &m_nWidth ) );
}
// XCloneable
//------------------------------------------------------------------------------
Reference< XCloneable > SAL_CALL OColumnControlModel::createClone( ) throw (RuntimeException)
{
    return new OColumnControlModel( this, getORB() ); 
}
//------------------------------------------------------------------------------
IMPLEMENT_TYPEPROVIDER2(OColumnControlModel,OColumnControlModel_BASE,comphelper::OPropertyContainer)
IMPLEMENT_PROPERTYCONTAINER_DEFAULTS(OColumnControlModel)
IMPLEMENT_SERVICE_INFO2_STATIC(OColumnControlModel,"com.sun.star.comp.dbu.OColumnControlModel","com.sun.star.awt.UnoControlModel","com.sun.star.sdb.ColumnDescriptorControlModel")
IMPLEMENT_FORWARD_REFCOUNT( OColumnControlModel, OColumnControlModel_BASE )
//------------------------------------------------------------------------------
Any SAL_CALL OColumnControlModel::queryInterface( const Type& _rType ) throw (RuntimeException)
{ 
	return OColumnControlModel_BASE::queryInterface( _rType );
}
// -----------------------------------------------------------------------------
// com::sun::star::XAggregation
Any SAL_CALL OColumnControlModel::queryAggregation( const Type& rType ) throw(RuntimeException)
{
	Any aRet(OColumnControlModel_BASE::queryAggregation(rType));
	if (!aRet.hasValue())
		aRet = comphelper::OPropertyContainer::queryInterface(rType);
	return aRet;
}
//------------------------------------------------------------------------------
::rtl::OUString SAL_CALL OColumnControlModel::getServiceName() throw ( RuntimeException)
{
	return ::rtl::OUString();
}
//------------------------------------------------------------------------------
void OColumnControlModel::write(const Reference<XObjectOutputStream>& /*_rxOutStream*/) throw ( ::com::sun::star::io::IOException, RuntimeException)
{
	// TODO
}

//------------------------------------------------------------------------------
void OColumnControlModel::read(const Reference<XObjectInputStream>& /*_rxInStream*/) throw ( ::com::sun::star::io::IOException, RuntimeException)
{
	// TODO
}

//.........................................................................
}	// namespace dbaui
//.........................................................................

