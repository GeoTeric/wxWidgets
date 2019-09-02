/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/slider.cpp
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/slider.h"
#include "wx/qt/private/converter.h"
#include "wx/qt/private/winevent.h"

#include <QtWidgets/QSlider>

namespace
{
    bool getInvertSlider(Qt::Orientation orientation, long style)
    {
        // When inverted, QT sliders seem to have their min value at the bottom, opposite to other platforms
        if (orientation == Qt::Vertical)
            return (style & wxSL_INVERSE) == 0;
        return style & wxSL_INVERSE;
    }
}

class wxQtSlider : public wxQtEventSignalHandler< QSlider, wxSlider >
{
public:
    wxQtSlider( wxWindow *parent, wxSlider *handler );

private:
    void valueChanged(int position);
};

wxQtSlider::wxQtSlider( wxWindow *parent, wxSlider *handler )
    : wxQtEventSignalHandler< QSlider, wxSlider >( parent, handler )
{
    connect(this, &QSlider::valueChanged, this, &wxQtSlider::valueChanged);
}

void wxQtSlider::valueChanged(int position)
{
    wxSlider *handler = GetHandler();
    if ( handler )
    {
        wxScrollEvent e(wxEVT_SCROLL_CHANGED, handler->GetId(), position,
                        wxQtConvertOrientation( orientation( ) ));
        EmitEvent( e );

        // and also generate a command event for compatibility
        wxCommandEvent event( wxEVT_SLIDER, handler->GetId() );
        event.SetInt( position );
        EmitEvent( event );
    }
}


wxSlider::wxSlider() :
    m_qtSlider(NULL)
{
}

wxSlider::wxSlider(wxWindow *parent,
         wxWindowID id,
         int value, int minValue, int maxValue,
         const wxPoint& pos,
         const wxSize& size,
         long style,
         const wxValidator& validator,
         const wxString& name)
{
    Create( parent, id, value, minValue, maxValue, pos, size, style, validator, name );
}

bool wxSlider::Create(wxWindow *parent,
            wxWindowID id,
            int WXUNUSED(value), int minValue, int maxValue,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtSlider = new wxQtSlider( parent, this );

    Qt::Orientation my_orientation = wxQtConvertOrientation(style, wxSL_HORIZONTAL);
    m_qtSlider->setOrientation(my_orientation);

    m_qtSlider->setInvertedAppearance(getInvertSlider(my_orientation, style));
    m_qtSlider->setInvertedControls(getInvertSlider(my_orientation, style));

    m_qtSlider->blockSignals(true);
    SetRange( minValue, maxValue );
    m_qtSlider->blockSignals(false);
    SetPageSize(wxMax(1, (maxValue - minValue) / 10));

#if 0 // there are not normally ticks for a wxSlider
    // draw ticks marks (default bellow if horizontal, right if vertical):
    if ( style & wxSL_VERTICAL )
    {
        m_qtSlider->setTickPosition( style & wxSL_LEFT ? QSlider::TicksLeft :
                                                         QSlider::TicksRight );
    }
    else // horizontal slider
    {
        m_qtSlider->setTickPosition( style & wxSL_TOP ? QSlider::TicksAbove :
                                                        QSlider::TicksBelow );
    }
#endif
    return QtCreateControl( parent, id, pos, size, style, validator, name );
}

int wxSlider::GetValue() const
{
    return m_qtSlider->value();
}

void wxSlider::SetValue(int value)
{
    m_qtSlider->blockSignals(true);
    m_qtSlider->setValue( value );
    m_qtSlider->blockSignals(false);
}

void wxSlider::SetRange(int minValue, int maxValue)
{
    m_qtSlider->blockSignals(true);
    m_qtSlider->setRange( minValue, maxValue );
    m_qtSlider->blockSignals(false);
}

int wxSlider::GetMin() const
{
    return m_qtSlider->minimum();
}

int wxSlider::GetMax() const
{
    return m_qtSlider->maximum();
}

void wxSlider::DoSetTickFreq(int freq)
{
    m_qtSlider->setTickInterval(freq);
}

int wxSlider::GetTickFreq() const
{
    return m_qtSlider->tickInterval();
}

void wxSlider::SetLineSize(int WXUNUSED(lineSize))
{
}

void wxSlider::SetPageSize(int pageSize)
{
    m_qtSlider->setPageStep(pageSize);
}

int wxSlider::GetLineSize() const
{
    return 0;
}

int wxSlider::GetPageSize() const
{
    return m_qtSlider->pageStep();
}

void wxSlider::SetThumbLength(int WXUNUSED(lenPixels))
{
}

int wxSlider::GetThumbLength() const
{
    return 0;
}


QWidget *wxSlider::GetHandle() const
{
    return m_qtSlider;
}

