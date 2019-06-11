/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/private/treeitemdelegate.h
// Purpose:     Delegate to create text edit controls for the tree items
// Author:      Matthew Griffin
// Created:     2019-05-29
// Copyright:   Matthew Griffin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TREEITEM_DELEGATE_H
#define _WX_TREEITEM_DELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

#include "wx/app.h"
#include "wx/textctrl.h"

#include "treeitemfactory.h"

class wxQTTreeItemDelegate : public QStyledItemDelegate
{
public:
    explicit wxQTTreeItemDelegate(wxWindow* parent)
        : m_parent(parent),
        m_textCtrl(NULL)
    {
    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &WXUNUSED(option), const QModelIndex &index) const wxOVERRIDE
    {
        m_current_model_index = index;
        m_textCtrl = new wxQtListTextCtrl(m_parent, parent);
        m_textCtrl->SetFocus();
        return m_textCtrl->GetHandle();
    }

    void destroyEditor(QWidget *WXUNUSED(editor), const QModelIndex &WXUNUSED(index)) const wxOVERRIDE
    {
        if (!wxTheApp->IsScheduledForDestruction(m_textCtrl))
        {
            m_current_model_index = QModelIndex();
            wxTheApp->ScheduleForDestruction(m_textCtrl);
            m_textCtrl = NULL;
        }
    }

    void setModelData(QWidget *WXUNUSED(editor), QAbstractItemModel *WXUNUSED(model), const QModelIndex &WXUNUSED(index)) const wxOVERRIDE
    {
        // Don't set model data until wx has had a chance to send out events
    }

    wxTextCtrl* GetEditControl() const
    {
        return m_textCtrl;
    }

    QModelIndex GetCurrentModelIndex() const
    {
        return m_current_model_index;
    }

    void AcceptModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }

private:
    wxWindow* m_parent;
    mutable wxTextCtrl* m_textCtrl;
    mutable QModelIndex m_current_model_index;
};

#endif // _WX_TREEITEM_DELEGATE_H