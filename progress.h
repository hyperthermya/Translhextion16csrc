#ifndef _PROGRESS_H
#define _PROGRESS_H

inline void SetProgressRange(HWND hwnd,long lowrange, long highrange)
{

	SendMessage(hwnd,PBM_SETRANGE32,lowrange,highrange);
}

inline void SetProgressStep(HWND hwnd,int step)
{
	SendMessage(hwnd,PBM_SETSTEP,step,0);
}

inline void ResetProgress(HWND hwnd)
{
	SendMessage(hwnd,PBM_SETPOS,0,0);
}

inline void StepProgress(HWND hwnd)
{
	SendMessage(hwnd,PBM_STEPIT,0,0);
}

inline void SetProgressPos(HWND hwnd, int pos)
{
	SendMessage(hwnd,PBM_SETPOS ,pos,0);
}

inline void SetProgressColor(HWND hwnd, COLORREF color)
{
	SendMessage(hwnd, PBM_SETBARCOLOR, 0, color);
}

class WaitCursor
{
public:
	WaitCursor()
	{
		SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	}

	~WaitCursor()
	{
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	}
};

#endif