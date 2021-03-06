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



#ifndef _VCL_ARRANGE_HXX
#define _VCL_ARRANGE_HXX

#include "vcl/window.hxx"

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace vcl
{
    /* some helper classes for simple window layouting
       guidelines:
       - a WindowArranger is not a Window
       - a WindowArranger hierarchy manages exactly one level of child windows inside a common parent
         this is to keep the vcl Window hierarchy flat, as some code like accelerators depend on such behavior
       - a WindowArranger never becomes owner of a Window, windows need to be destroyed separately
       - a WindowArranger however always is owner of its child WindowArrangers, that is the
         WindowArranger hierarchy will keep track of its objects and delete them
       - a managed element of a WindowArranger can either be a Window (a leaf in the hierarchy)
         or a child WindowArranger (a node in the hierarchy), but never both
    */

    class VCL_DLLPUBLIC WindowArranger
    {
    protected:
        struct Element
        {
            Window*                            m_pElement;
            boost::shared_ptr<WindowArranger>  m_pChild;
            sal_Int32                          m_nExpandPriority;
            Size                               m_aMinSize;
            bool                               m_bHidden;
            long                               m_nLeftBorder;
            long                               m_nTopBorder;
            long                               m_nRightBorder;
            long                               m_nBottomBorder;

            Element()
            : m_pElement( NULL )
            , m_pChild()
            , m_nExpandPriority( 0 )
            , m_bHidden( false )
            , m_nLeftBorder( 0 )
            , m_nTopBorder( 0 )
            , m_nRightBorder( 0 )
            , m_nBottomBorder( 0 )
            {}

            Element( Window* i_pWin,
                     boost::shared_ptr<WindowArranger> const & i_pChild = boost::shared_ptr<WindowArranger>(),
                     sal_Int32 i_nExpandPriority = 0,
                     const Size& i_rMinSize = Size()
                   )
            : m_pElement( i_pWin )
            , m_pChild( i_pChild )
            , m_nExpandPriority( i_nExpandPriority )
            , m_aMinSize( i_rMinSize )
            , m_bHidden( false )
            , m_nLeftBorder( 0 )
            , m_nTopBorder( 0 )
            , m_nRightBorder( 0 )
            , m_nBottomBorder( 0 )
            {}

            void deleteChild() { m_pChild.reset(); }
            
            sal_Int32 getExpandPriority() const;
            Size getOptimalSize( WindowSizeType ) const;
            bool isVisible() const;
            void setPosSize( const Point&, const Size& );
        };

        Window*                     m_pParentWindow;
        WindowArranger*             m_pParentArranger;
        Rectangle                   m_aManagedArea;
        long                        m_nOuterBorder;
        
        rtl::OUString               m_aIdentifier;
        
        virtual Element* getElement( size_t i_nIndex ) = 0;
        const Element* getConstElement( size_t i_nIndex ) const
        { return const_cast<WindowArranger*>(this)->getElement( i_nIndex ); }
 

    public:
        static long getDefaultBorder();
        
        static long getBorderValue( long nBorder )
        { return nBorder >= 0 ? nBorder : -nBorder * getDefaultBorder(); }
        
        WindowArranger( WindowArranger* i_pParent = NULL )
        : m_pParentWindow( i_pParent ? i_pParent->m_pParentWindow : NULL )
        , m_pParentArranger( i_pParent )
        , m_nOuterBorder( 0 )
        {}
        virtual ~WindowArranger();

        // ask what would be the optimal size
        virtual Size getOptimalSize( WindowSizeType ) const = 0;
        // call Resize to trigger layouting inside the managed area
        // without function while parent window is unset
        virtual void resize() = 0;
        // avoid this if possible, using the constructor instead
        // there can be only one parent window and all managed windows MUST
        // be direct children of that window
        // violating that condition will result in undefined behavior
        virtual void setParentWindow( Window* );
        
        virtual void setParent( WindowArranger* );

        virtual size_t countElements() const = 0;
        boost::shared_ptr<WindowArranger> getChild( size_t i_nIndex ) const
        {
            const Element* pEle = getConstElement( i_nIndex );
            return pEle ? pEle->m_pChild : boost::shared_ptr<WindowArranger>();
        }
        Window* getWindow( size_t i_nIndex ) const
        {
            const Element* pEle = getConstElement( i_nIndex );
            return pEle ? pEle->m_pElement : NULL;
        }
        
        virtual bool isVisible() const; // true if any element is visible
        
        virtual com::sun::star::uno::Sequence< com::sun::star::beans::PropertyValue > getProperties() const;
        virtual void setProperties( const com::sun::star::uno::Sequence< com::sun::star::beans::PropertyValue >& );
        
        sal_Int32 getExpandPriority( size_t i_nIndex ) const
        {
            const Element* pEle = getConstElement( i_nIndex );
            return pEle ? pEle->getExpandPriority() : 0;
        }
        
        Size getMinimumSize( size_t i_nIndex ) const
        {
            const Element* pEle = getConstElement( i_nIndex );
            return pEle ? pEle->m_aMinSize : Size();
        }
        
        bool setMinimumSize( size_t i_nIndex, const Size& i_rMinSize )
        {
            Element* pEle = getElement( i_nIndex );
            if( pEle )
                pEle->m_aMinSize = i_rMinSize;
            return pEle != NULL;
        }

        void setBorders( size_t i_nIndex, long i_nLeft, long i_nTop, long i_nRight, long i_nBottom  )
        {
            Element* pEle = getElement( i_nIndex );
            if( pEle )
            {
                pEle->m_nLeftBorder   = i_nLeft;
                pEle->m_nRightBorder  = i_nRight;
                pEle->m_nTopBorder    = i_nTop;
                pEle->m_nBottomBorder = i_nBottom;
            }
        }
        
        void getBorders( size_t i_nIndex, long* i_pLeft = NULL, long* i_pTop = NULL, long* i_pRight = NULL, long* i_pBottom = NULL ) const
        {
            const Element* pEle = getConstElement( i_nIndex );
            if( pEle )
            {
                if( i_pLeft )   *i_pLeft   = pEle->m_nLeftBorder;
                if( i_pTop )    *i_pTop    = pEle->m_nTopBorder;
                if( i_pRight )  *i_pRight  = pEle->m_nRightBorder;
                if( i_pBottom ) *i_pBottom = pEle->m_nBottomBorder;
            }
        }
        
        
        void show( bool i_bShow = true, bool i_bImmediateUpdate = true );

        void setManagedArea( const Rectangle& i_rArea )
        {
            m_aManagedArea = i_rArea;
            resize();
        }
        const Rectangle& getManagedArea() const { return m_aManagedArea; }

        void setOuterBorder( long i_nBorder )
        {
            m_nOuterBorder = i_nBorder;
            resize();
        }
        
        const rtl::OUString getIdentifier() const
        { return m_aIdentifier; }
        
        void setIdentifier( const rtl::OUString& i_rId )
        { m_aIdentifier = i_rId; }
    };

    class VCL_DLLPUBLIC RowOrColumn : public WindowArranger
    {
        long    m_nBorderWidth;
        bool    m_bColumn;

        std::vector< WindowArranger::Element > m_aElements;

        void distributeRowWidth( std::vector< Size >& io_rSizes, long i_nUsedWidth, long i_nExtraWidth );
        void distributeColumnHeight( std::vector< Size >& io_rSizes, long i_nUsedHeight, long i_nExtraHeight );
    protected:
        virtual Element* getElement( size_t i_nIndex )
        { return i_nIndex < m_aElements.size() ? &m_aElements[ i_nIndex ] : 0; }

    public:
        RowOrColumn( WindowArranger* i_pParent = NULL,
                     bool bColumn = true, long i_nBorderWidth = -1 )
        : WindowArranger( i_pParent )
        , m_nBorderWidth( i_nBorderWidth )
        , m_bColumn( bColumn )
        {}

        virtual ~RowOrColumn();

        virtual Size getOptimalSize( WindowSizeType ) const;
        virtual void resize();
        virtual size_t countElements() const { return m_aElements.size(); }

        // add a managed window at the given index
        // an index smaller than zero means add the window at the end
        size_t addWindow( Window*, sal_Int32 i_nExpandPrio = 0, const Size& i_rMinSize = Size(), size_t i_nIndex = ~0 );
        void remove( Window* );

        size_t addChild( boost::shared_ptr<WindowArranger> const &, sal_Int32 i_nExpandPrio = 0, size_t i_nIndex = ~0 );
        // convenience: use for addChild( new WindowArranger( ... ) ) constructs
        size_t addChild( WindowArranger* i_pNewChild, sal_Int32 i_nExpandPrio = 0, size_t i_nIndex = ~0 )
        { return addChild( boost::shared_ptr<WindowArranger>( i_pNewChild ), i_nExpandPrio, i_nIndex ); }
        void remove( boost::shared_ptr<WindowArranger> const & );
        
        long getBorderWidth() const { return m_nBorderWidth; }
    };

    class VCL_DLLPUBLIC LabeledElement : public WindowArranger
    {
        WindowArranger::Element m_aLabel;
        WindowArranger::Element m_aElement;
        long                    m_nDistance;
        long                    m_nLabelColumnWidth;
        int                     m_nLabelStyle;
    protected:
        virtual Element* getElement( size_t i_nIndex )
        {
            if( i_nIndex == 0 )
                return &m_aLabel;
            else if( i_nIndex == 1 )
                return &m_aElement;
            return 0;
        }

    public:
        LabeledElement( WindowArranger* i_pParent = NULL, int i_nLabelStyle = 0, long i_nDistance = -1 )
        : WindowArranger( i_pParent )
        , m_nDistance( i_nDistance )
        , m_nLabelColumnWidth( 0 )
        , m_nLabelStyle( i_nLabelStyle )
        {}

        virtual ~LabeledElement();

        virtual Size getOptimalSize( WindowSizeType ) const;
        virtual void resize();
        virtual size_t countElements() const { return 2; }

        void setLabel( Window* );
        void setLabel( boost::shared_ptr<WindowArranger> const & );
        void setElement( Window* );
        void setElement( boost::shared_ptr<WindowArranger> const & );
        void setLabelColumnWidth( long i_nWidth )
        { m_nLabelColumnWidth = i_nWidth; }
        
        Size getLabelSize( WindowSizeType i_eType ) const
        { return m_aLabel.getOptimalSize( i_eType ); }
        Size getElementSize( WindowSizeType i_eType ) const
        { return m_aElement.getOptimalSize( i_eType ); }
    };
    
    class VCL_DLLPUBLIC LabelColumn : public RowOrColumn
    {
        long getLabelWidth() const;
    public:
        LabelColumn( WindowArranger* i_pParent = NULL, long i_nBorderWidth = -1 )
        : RowOrColumn( i_pParent, true, i_nBorderWidth )
        {}
        virtual ~LabelColumn();
        
        virtual Size getOptimalSize( WindowSizeType ) const;
        virtual void resize();
        
        // returns the index of the added label
        size_t addRow( Window* i_pLabel, boost::shared_ptr<WindowArranger> const& i_rElement, long i_nIndent = 0 );
        size_t addRow( Window* i_pLabel, Window* i_pElement, long i_nIndent = 0, const Size& i_rElementMinSize = Size() );
    };

    class VCL_DLLPUBLIC Indenter : public WindowArranger
    {
        long                        m_nIndent;
        WindowArranger::Element     m_aElement;

    protected:
        virtual Element* getElement( size_t i_nIndex )
        { return i_nIndex == 0 ? &m_aElement : NULL; }

    public:
        Indenter( WindowArranger* i_pParent = NULL, long i_nIndent = 3*getDefaultBorder() )
        : WindowArranger( i_pParent )
        , m_nIndent( i_nIndent )
        {}

        virtual ~Indenter();

        virtual Size getOptimalSize( WindowSizeType ) const;
        virtual void resize();
        virtual size_t countElements() const { return (m_aElement.m_pElement != 0 || m_aElement.m_pChild != 0) ? 1 : 0; }

        void setIndent( long i_nIndent )
        {
            m_nIndent = i_nIndent;
            resize();
        }

        void setWindow( Window*, sal_Int32 i_nExpandPrio = 0 );
        void setChild( boost::shared_ptr<WindowArranger> const &, sal_Int32 i_nExpandPrio = 0 );
        // convenience: use for setChild( new WindowArranger( ... ) ) constructs
        void setChild( WindowArranger* i_pChild, sal_Int32 i_nExpandPrio = 0 )
        { setChild( boost::shared_ptr<WindowArranger>( i_pChild ), i_nExpandPrio ); }
    };
    
    class VCL_DLLPUBLIC Spacer : public WindowArranger
    {
        WindowArranger::Element     m_aElement;
        Size                        m_aSize;

    protected:
        virtual Element* getElement( size_t i_nIndex )
        { return i_nIndex == 0 ? &m_aElement : NULL; }

    public:
        Spacer( WindowArranger* i_pParent = NULL, sal_Int32 i_nPrio = 20, const Size& i_rSize = Size( 0, 0 ) )
        : WindowArranger( i_pParent )
        , m_aElement( NULL, boost::shared_ptr<WindowArranger>(), i_nPrio )
        , m_aSize( i_rSize )
        {}

        virtual ~Spacer() {}

        virtual Size getOptimalSize( WindowSizeType ) const
        { return m_aSize; }
        virtual void resize() {}
        virtual void setParentWindow( Window* ) {}
        virtual size_t countElements() const { return 1; }
        virtual bool isVisible() const { return true; }
    };

    class VCL_DLLPUBLIC MatrixArranger : public WindowArranger
    {
        long    m_nBorderX;
        long    m_nBorderY;
        
        struct MatrixElement : public WindowArranger::Element
        {
            sal_uInt32  m_nX;
            sal_uInt32  m_nY;
            
            MatrixElement()
            : WindowArranger::Element()
            , m_nX( 0 )
            , m_nY( 0 )
            {}
            
            MatrixElement( Window* i_pWin,
                           sal_uInt32 i_nX, sal_uInt32 i_nY,
                           boost::shared_ptr<WindowArranger> const & i_pChild = boost::shared_ptr<WindowArranger>(),
                           sal_Int32 i_nExpandPriority = 0,
                           const Size& i_rMinSize = Size()
                          )
            : WindowArranger::Element( i_pWin, i_pChild, i_nExpandPriority, i_rMinSize )
            , m_nX( i_nX )
            , m_nY( i_nY )
            {
            }
        };

        std::vector< MatrixElement >            m_aElements;
        std::map< sal_uInt64, size_t >          m_aMatrixMap;  // maps (x | (y << 32)) to index in m_aElements
        
        sal_uInt64 getMap( sal_uInt32 i_nX, sal_uInt32 i_nY )
        { return static_cast< sal_uInt64 >(i_nX) | (static_cast< sal_uInt64>(i_nY) << 32 ); }
        
        static void distributeExtraSize( std::vector<long>& io_rSizes, const std::vector<sal_Int32>& i_rPrios, long i_nExtraWidth );

        Size getOptimalSize( WindowSizeType,
                             std::vector<long>& o_rColumnWidths, std::vector<long>& o_rRowHeights,
                             std::vector<sal_Int32>& o_rColumnPrio, std::vector<sal_Int32>& o_rRowPrio
                            ) const;
    protected:
        virtual Element* getElement( size_t i_nIndex )
        { return i_nIndex < m_aElements.size() ? &m_aElements[ i_nIndex ] : 0; }

    public:
        MatrixArranger( WindowArranger* i_pParent = NULL,
                        long i_nBorderX = -1,
                        long i_nBorderY = -1 )
        : WindowArranger( i_pParent )
        , m_nBorderX( i_nBorderX )
        , m_nBorderY( i_nBorderY )
        {}

        virtual ~MatrixArranger();

        virtual Size getOptimalSize( WindowSizeType ) const;
        virtual void resize();
        virtual size_t countElements() const { return m_aElements.size(); }

        // add a managed window at the given matrix position
        size_t addWindow( Window*, sal_uInt32 i_nX, sal_uInt32 i_nY, sal_Int32 i_nExpandPrio = 0, const Size& i_rMinSize = Size() );
        void remove( Window* );

        size_t addChild( boost::shared_ptr<WindowArranger> const &, sal_uInt32 i_nX, sal_uInt32 i_nY, sal_Int32 i_nExpandPrio = 0 );
        // convenience: use for addChild( new WindowArranger( ... ) ) constructs
        size_t addChild( WindowArranger* i_pNewChild, sal_uInt32 i_nX, sal_uInt32 i_nY, sal_Int32 i_nExpandPrio = 0 )
        { return addChild( boost::shared_ptr<WindowArranger>( i_pNewChild ), i_nX, i_nY, i_nExpandPrio ); }
        void remove( boost::shared_ptr<WindowArranger> const & );
    };

}

#endif

