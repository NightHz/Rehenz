// PrintfAsyn.h
//
// By NightHz
//
// 控制台异步输出，防止阻塞
//

#ifndef _PRINTFASYN_H_
#define _PRINTFASYN_H_


bool CreatePrintfAsynThread();
void PrintfAsyn(const char * _string);
void PrintfAsynFlush();


#endif // !_PRINTFASYN_H_
