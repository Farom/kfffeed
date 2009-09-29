project(kfffeed)

# search packages used by KDE
find_package (KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(CheckIncludeFiles)
find_package (KdepimLibs REQUIRED)

if(WIN32)
    set(LIB_INSTALL_DIR
                        ${LIB_INSTALL_DIR}
                        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
                        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
                        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} )
endif(WIN32)

include_directories(
    ${KDE4_INCLUDES}
    ${KDEPIMLIBS_INCLUDE_DIRS}
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS} -O0 -g" )

########### next target ###############

set( kfffeed_SRCS main.cpp
    fritzboxphonenumber.h
    fritzboxphonenumber.cpp
    fritzboxphonebookcontact.h
    fritzboxphonebookcontact.cpp
    fritzboxphonebook.h
    fritzboxphonebook.cpp
    qphonenumberstring.h
    qphonenumberstring.cpp
    environment.h
    environment.cpp)

kde4_add_executable(kfffeed NOGUI ${kfffeed_SRCS})

target_link_libraries(kfffeed ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY}
${KDE4_KDECORE_LIBS} ${KDEPIMLIBS_KABC_LIBS})

ADD_CUSTOM_COMMAND(
        OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/../doc/kfffeed.1
        COMMAND ${CMAKE_COMMAND} xsltproc /usr/share/xml/docbook/stylesheet/nwalsh/xhtml/docbook.xsl ${CMAKE_CURRENT_SOURCE_DIR}/../doc/kfffeed.manpage.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../doc/kfffeed.manpage.xml
             )

install(TARGETS kfffeed DESTINATION bin)
install(FILES ../doc/kfffeed.1  DESTINATION share/man)
install(FILES ../data/netnumbers.xml  DESTINATION share/kfffeed)


