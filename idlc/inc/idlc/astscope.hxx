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


#ifndef _IDLC_ASTSCOPE_HXX_
#define _IDLC_ASTSCOPE_HXX_

#include <idlc/idlc.hxx>

class AstExpression;
#include <idlc/astdeclaration.hxx>
#include <idlc/astexpression.hxx>

class AstScope
{
public:
	AstScope(NodeType nodeType);
	virtual ~AstScope();

	NodeType getScopeNodeType() const
		{ return m_nodeType; }

	virtual AstDeclaration* addDeclaration(AstDeclaration* pDecl);

	sal_uInt32 nMembers() const
		{ return (sal_uInt32)(m_declarations.size()); }
	DeclList::const_iterator getIteratorBegin() const
		{ return m_declarations.begin(); }
	DeclList::const_iterator getIteratorEnd() const
		{ return m_declarations.end(); }
	sal_uInt16 getNodeCount(NodeType nType);

	// Name look up mechanism
	AstDeclaration* lookupByName(const ::rtl::OString& scopedName);
	// Look up the identifier 'name' specified only in the local scope
	AstDeclaration* lookupByNameLocal(const ::rtl::OString& name) const;

	AstDeclaration* lookupInForwarded(const ::rtl::OString& scopedName);
	AstDeclaration* lookupInInherited(const ::rtl::OString& scopedName) const;

	// Look up a predefined type by its ExprType
	AstDeclaration* lookupPrimitiveType(ExprType type);

	AstDeclaration* lookupForAdd(AstDeclaration* pDecl);

protected:
    inline AstDeclaration const * getLast() const
    { return m_declarations.back(); }

private:
	DeclList	m_declarations;
	NodeType 	m_nodeType;
};

#endif // _IDLC_ASTSCOPE_HXX_

