Author:  Bill Waller <BillWaller@wallerbroadcasting.com>
Copyright (c) 2015 Bill Waller

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-------------------------------------------------------------------------

sg.c

Synopsis:  switch group id or switch to root user
           can be used as system back door

sg [group-name|goroot] args

Examples

To start a new shell with a specified group ID:

# sg uucp

To start an interactive shell with root authority:

# sg goroot

To execute a shell script with root authority:

# sg goroot -c ./myscript.sh

To execute a system command with root authority:

# sg goroot -c "/bin/ls -l"

-------------------------------------------------------------------------
