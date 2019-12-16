#pragma once
#ifndef GLERROR_H
#define GLERROR_H

void _check_gl_error(const char* file, int line);

#define check_gl_error() _check_gl_error(_FILE_,_LINE_)

#endif // !GLERROR_H

