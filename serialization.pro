TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    codegen.cpp

DISTFILES += \
    third_party/rapidxml/license.txt \
    CMakeLists.txt

HEADERS += \
    detail/objects.hpp \
    detail/xmlcppgenerator.hpp \
    detail/xmlparser.hpp \
    third_party/rapidxml/rapidxml.hpp \
    third_party/rapidxml/rapidxml_iterators.hpp \
    third_party/rapidxml/rapidxml_print.hpp \
    third_party/rapidxml/rapidxml_utils.hpp \
    codegen.h \
    config.h \
    third_party/rapidjson/error/en.h \
    third_party/rapidjson/error/error.h \
    third_party/rapidjson/internal/biginteger.h \
    third_party/rapidjson/internal/diyfp.h \
    third_party/rapidjson/internal/dtoa.h \
    third_party/rapidjson/internal/ieee754.h \
    third_party/rapidjson/internal/itoa.h \
    third_party/rapidjson/internal/meta.h \
    third_party/rapidjson/internal/pow10.h \
    third_party/rapidjson/internal/regex.h \
    third_party/rapidjson/internal/stack.h \
    third_party/rapidjson/internal/strfunc.h \
    third_party/rapidjson/internal/strtod.h \
    third_party/rapidjson/internal/swap.h \
    third_party/rapidjson/msinttypes/inttypes.h \
    third_party/rapidjson/msinttypes/stdint.h \
    third_party/rapidjson/allocators.h \
    third_party/rapidjson/document.h \
    third_party/rapidjson/encodedstream.h \
    third_party/rapidjson/encodings.h \
    third_party/rapidjson/filereadstream.h \
    third_party/rapidjson/filewritestream.h \
    third_party/rapidjson/fwd.h \
    third_party/rapidjson/istreamwrapper.h \
    third_party/rapidjson/memorybuffer.h \
    third_party/rapidjson/memorystream.h \
    third_party/rapidjson/ostreamwrapper.h \
    third_party/rapidjson/pointer.h \
    third_party/rapidjson/prettywriter.h \
    third_party/rapidjson/rapidjson.h \
    third_party/rapidjson/reader.h \
    third_party/rapidjson/schema.h \
    third_party/rapidjson/stream.h \
    third_party/rapidjson/stringbuffer.h \
    third_party/rapidjson/writer.h \
    detail/jsoncppgenerator.hpp

INCLUDEPATH += ./third_party
