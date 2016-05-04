# CompiledStringDecompilor
Similler function with https://github.com/SinaMafuyu/String_Client_Compiler <br/>
helps decompile compiled String file. <br/><br/>

CSD Supports Scripts (.sds file)<br/><br/>

Main fuction is read data by format in .sds file.<br/><br/>

# .sds Scripts
supports comment before sds version string<br/>
supports Variable, can set essential or not, ST dependence, ID and ST option. <br/>
can set several option by Scripts. - not equal, equal, bigger than, less then.<br/>
if nequal, equal setted, equal affect by AND option.<br/>
in format string, all number is HEX, same as Var option.<br/>
by adding $, can set ST dependence (if there is no ST, ST-Dependence will not appears)<br/><br/>

sds:v1.0<br/>
idSize strSize                - increase only program says there is no enough memory<br/>
0A #a ID $12 #c ST 14         - #: Var, $: ST dependence HEX, ST: String, ID: id<br/><br/>

#c $                          - ST dependence<br/>
#a ~ 0                        - a is not essential, default is 0<br/>
#a >= 3                       - you should set option when Var is not essential<br/>
#a != 0A                      - a nequl 0x0A<br/>
#c < 20                       - c < 0x20<br/>
#c ~ 20                       - ST dependence is not means non-essential<br/>
