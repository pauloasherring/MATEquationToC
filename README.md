# MATLABToC
A simple tool for converting MATLAB equations (with ^ symbols and numeric literals without decimal dots) to C.
Requires: 
  Qt5+ due to QRegularExpression
  
Use at your own risk. These routines have not been thoroughly tested.
Before accepting that the equation's integrity is mantained, replace pow() calls for power() and compare it at MATLAB.
