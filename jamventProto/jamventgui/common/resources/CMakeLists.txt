# Remove *.qrc, not building in  all envs.
# file(GLOB SRCS . *.cpp *.h *.ui *.qrc)
file(GLOB SRCS . *.cpp *.h *.ui)
set (GUI_SOURCES ${GUI_SOURCES} ${SRCS} PARENT_SCOPE)
