
sourceFiles = [
  'LoanCalculator.cpp',
  'LoanCalcQtMainWindow.cpp',
  'LoanCalculatorMain.cpp'
]

ccflags = [
  '-O2',
  '-Wall',
  '-Werror',
# Since they're system includes, just put them in the CCFLAGS and not the CPPPATH
  '-I/usr/include/qt4',
  '-I/usr/include/qt4/QtGui'
]

libs = [
  'QtGui',
  'QtCore',
  'pthread',
  'CmdLineParser'
]

qtDefines = [
  '_REENTRANT',
  'QT_NO_DEBUG',
  'QT_GUI_LIB',
  'QT_CORE_LIB',
  'QT_SHARED'
]

env = Environment(tools=['default','qt'])

env.Append(CPPPATH = ['.', '../cmdLineParser'],
           CCFLAGS = ccflags,
           CPPDEFINES = qtDefines,
           LIBPATH = '../cmdLineParser')
env.Replace(LIBS = libs) # Have to do this since SCons was adding -lqt which I cant find

# SCons automatically generates MOC files when necessary after having set 'qt' in the tools
env.Program(target = 'loanCalculator', source = sourceFiles)
