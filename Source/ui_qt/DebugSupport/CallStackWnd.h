#pragma once

#include "signal/Signal.h"
// #include "win32/MDIChild.h"
// #include "win32/ListView.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include "Types.h"
#include "MIPS.h"
#include "VirtualMachineStateView.h"

class CBiosDebugInfoProvider;

class CCallStackWnd : public QMdiSubWindow, public CVirtualMachineStateView
{
public:
	typedef Framework::CSignal<void(uint32)> OnFunctionDblClickSignal;

	CCallStackWnd(QMdiArea*, CMIPS*, CBiosDebugInfoProvider*);
	virtual ~CCallStackWnd();

	void HandleMachineStateChange() override;

	OnFunctionDblClickSignal OnFunctionDblClick;

private:
	void Update();

	CMIPS* m_context;
	CBiosDebugInfoProvider* m_biosDebugInfoProvider;

	QListWidget *m_listWidget;
};
