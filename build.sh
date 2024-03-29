#!/bin/bash

#
# Step 1: Detect the operating system
#
MAC="Mac OS X"
WIN="Windows"
LIN="Linux"

if [ `uname` = "Darwin" ]; then
    OS=$MAC
elif [ `uname` = "Linux" ]; then
    OS=$LIN
else
    OS=$WIN
fi

# Move to src dir
APP_PATH="${0/C://c}"
APP_PATH="${APP_PATH//\\//}"
APP_PATH=`echo $APP_PATH | awk '{split($0,patharr,"/"); idx=1; while(patharr[idx+1] != "") { if (patharr[idx] != "/") {printf("%s/", patharr[idx]); idx++ }} }'`
APP_PATH=`cd "$APP_PATH"; pwd` 
cd "$APP_PATH"

GAME_NAME=${APP_PATH##*/}
FULL_APP_PATH=$APP_PATH
APP_PATH="."

LIBSRC_DIR="${APP_PATH}/lib"

#Set the basic paths
OUT_DIR="${APP_PATH}/bin"
FULL_OUT_DIR="${FULL_APP_PATH}/bin"
TMP_DIR="${APP_PATH}/tmp"
SRC_DIR="${APP_PATH}/src"

LOG_FILE="${APP_PATH}/out.log"

C_FLAGS="-x c++"
SG_INC="-I${APP_PATH}/lib/"

if [ "$OS" = "$MAC" ]; then
    ICON="SwinGame.icns"
else
    ICON="SwinGame"
fi

if [ "$OS" = "$WIN" ]; then
    C_FLAGS="$C_FLAGS -march=i386"
fi

#Locate the compiler...
GCC_BIN=`which clang 2>> /dev/null`
if [ -z "$GCC_BIN" ]; then
    #try locating gcc
    GCC_BIN=`which g++`
    
    if [ -z "$GCC_BIN" ]; then
        #no compiler found :(
        echo "Unable to find a C compiler. Install either clang or gcc."
        exit -1
    fi
fi

CLEAN="N"

#
# Library versions
#
OPENGL=false
SDL_13=false

Usage()
{
    echo "Usage: [-c] [-h] [-r] [name]"
    echo 
    echo "Compiles your game into an executable application."
    echo "Output is located in $OUT_DIR."
    echo
    echo "Options:"
    echo " -c   Perform a clean rather than a build"
    echo " -h   Show this help message"
    echo " -r   Create a release build"
    exit 0
}

RELEASE=""

while getopts chrg:b: o
do
    case "$o" in
    c)  CLEAN="Y" ;;
    b)  if [ "${OPTARG}" = "adass" ]; then
            SDL_13=true
            OPENGL=false
        fi 
        ;;
    h)  Usage ;;
    g)  if [ "${OPTARG}" = "odly" ]; then
            SDL13=false
            OPENGL=true
        fi 
        ;;
    r)  RELEASE="Y" ;;
    ?)  Usage
    esac
done

shift $((${OPTIND}-1))

if [ "a$1a" != "aa" ]; then
    GAME_NAME=$1
fi

#
# Change directories based on release or debug builds
#

if [ -n "${RELEASE}" ]; then
    C_FLAGS="${C_FLAGS} -O3 -Wall"
    OUT_DIR="${OUT_DIR}/Release"
    FULL_OUT_DIR="${FULL_OUT_DIR}/Release"
    TMP_DIR="${TMP_DIR}/Release"
else
    C_FLAGS="${C_FLAGS} -g -Wall"
    OUT_DIR="${OUT_DIR}/Debug"
    FULL_OUT_DIR="${FULL_OUT_DIR}/Debug"
    TMP_DIR="${TMP_DIR}/Debug"
fi

if [ "$OS" = "$MAC" ]; then
    if [ ${SDL_13} = true ]; then
      TMP_DIR="${TMP_DIR}/badass"
      LIB_DIR="${APP_PATH}/lib/sdl13/mac"
    elif [ ${OPENGL} = true ]; then
        TMP_DIR="${TMP_DIR}/godly"
      LIB_DIR="${APP_PATH}/lib/godly/mac"
    else
      TMP_DIR="${TMP_DIR}/sdl12"
      LIB_DIR="${APP_PATH}/lib/mac"
    fi
elif [ "$OS" = "$WIN" ]; then
    #
    # This needs 1.3 versions of SDL for Windows...
    # along with function sdl_gfx, sdl_ttf, sdl_image, sdl_mixer
    #
    
    # if [ ${SDL_13} = true ]; then
    #   LIB_DIR="${APP_PATH}/lib/sdl13/win"
    # elif [ ${OPENGL} = true ]; then
    #   LIB_DIR="${APP_PATH}/lib/sdl13/win"
    # else
    SDL_13=false
    OPENGL=false
    LIB_DIR="${APP_PATH}/lib/win"
    # fi
else #linux
    LIB_DIR="/usr/lib"
fi

if [ -f "${LOG_FILE}" ]
then
    rm -f "${LOG_FILE}"
fi

DoDriverMessage()
{
  if [ ${SDL_13} = true ]; then
    echo "  ... Using SDL 1.3 Driver"
  elif [ ${OPENGL} = true ]; then
    echo "  ... Using OpenGL Driver"
  else
    echo "  ... Using SDL 1.2 Driver"
  fi
}

CleanTmp()
{
    if [ -d "${TMP_DIR}" ]
    then
        rm -rf "${TMP_DIR}"
    fi
    mkdir "${TMP_DIR}"
}

#
# Compile the passed in file
# $1 = filename
# $2 = name
# $3 = out filename
# $4 = extra options
#
doCompile()
{
    file=$1
    name=$2
    out_file=$3
    extra_opts=$4
    
    if [ ! -f $out_file ] || [ $file -nt $out_file ]; then
        echo "      ... Compiling ${name}"    
        ${GCC_BIN} -c ${extra_opts} ${SG_INC} ${C_FLAGS} -o "${out_file}" "${file}" >> ${LOG_FILE}
        if [ $? != 0 ]; then echo "Error compiling"; cat ${LOG_FILE}; exit 1; fi
    fi
}

# $1 = other opts
doCompileGameMain()
{
    name="${SRC_DIR}/${GAME_MAIN}"
    name=${name##*/} # ## = delete longest match for */... ie all but file name
    name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp
    doCompile "${SRC_DIR}/${GAME_MAIN}" "${name}" "${TMP_DIR}/${name}.o" "$1"
}

doBasicMacCompile()
{
    mkdir -p "${TMP_DIR}"
    for file in `find ${LIBSRC_DIR} | grep [.]cpp$` ; do
        name=${file##*/} # ## = delete longest match for */... ie all but file name
        name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp
        out_file="${TMP_DIR}/${name}.o"
        doCompile "${file}" "${name}" "${out_file}" "-arch i386"

    done
    
    doCompileGameMain "-arch i386"
    
    #Assemble all of the .s files
    echo "  ... Creating game"
    FRAMEWORKS=`ls -d ${LIB_DIR}/*.framework | awk -F . '{split($2,patharr,"/"); idx=1; while(patharr[idx+1] != "") { idx++ } printf("-framework %s ", patharr[idx]) }'`
    
    ${GCC_BIN} -F${LIB_DIR} ${FRAMEWORKS} -Wl,-rpath,@loader_path/../Frameworks -arch i386 -o "${OUT_DIR}/${GAME_NAME}" `find ${TMP_DIR}/${1} -maxdepth 1 -name \*.o`
    if [ $? != 0 ]; then echo "Error creating game"; exit 1; fi
}

#
# Compile for Mac - manually assembles and links files
# argument 1 is arch
#
doMacCompile()
{
    mkdir -p "${TMP_DIR}/${1}"
    
    echo "  ... Compiling for $1"
    for file in `find ${LIBSRC_DIR} | grep [.]cpp$` ; do
        name=${file##*/} # ## = delete longest match for */... ie all but file name
        name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp
        out_file="${TMP_DIR}/${1}/${name}.o"
        doCompile "${file}" "${name}" "${out_file}" "-arch ${1}"
    done
    
    doCompileGameMain "-arch ${1}"
    
    #Assemble all of the .s files
    echo "  ... Creating game for $1"
    FRAMEWORKS=`ls -d ${LIB_DIR}/*.framework | awk -F . '{split($2,patharr,"/"); idx=1; while(patharr[idx+1] != "") { idx++ } printf("-framework %s ", patharr[idx]) }'`
    
    ${GCC_BIN} -F${LIB_DIR} ${FRAMEWORKS} -Wl,-rpath,@loader_path/../Frameworks -arch $1 $2 -o "${TMP_DIR}/${1}/${GAME_NAME}" `find ${TMP_DIR}/${1} -maxdepth 1 -name \*.o`
    
    if [ $? != 0 ]; then echo "Error creating game"; cat ${LOG_FILE}; exit 1; fi
}

# 
# Create fat executable (i386 + ppc)
# 
doLipo()
{
    echo "  ... Creating Universal Binary"
    lipo -arch ${1} "${TMP_DIR}/${1}/${GAME_NAME}" -arch ${2} "${TMP_DIR}/${2}/${GAME_NAME}" -output "${OUT_DIR}/${GAME_NAME}" -create
}

doMacPackage()
{
    GAMEAPP_PATH="${FULL_OUT_DIR}/${GAME_NAME}.app"
    if [ -d "${GAMEAPP_PATH}" ]; then
        echo "  ... Removing old application"
        rm -rf "${GAMEAPP_PATH}"
    fi

    echo "  ... Creating Application Bundle"
    
    mkdir "${GAMEAPP_PATH}"
    mkdir "${GAMEAPP_PATH}/Contents"
    mkdir "${GAMEAPP_PATH}/Contents/MacOS"
    mkdir "${GAMEAPP_PATH}/Contents/Resources"
    mkdir "${GAMEAPP_PATH}/Contents/Frameworks"

    echo "  ... Added Private Frameworks"
    cp -R -p "${LIB_DIR}/"*.framework "${GAMEAPP_PATH}/Contents/Frameworks/"

    mv "${OUT_DIR}/${GAME_NAME}" "${GAMEAPP_PATH}/Contents/MacOS/" 
    echo "<?xml version='1.0' encoding='UTF-8'?>\
    <!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\
    <plist version=\"1.0\">\
    <dict>\
            <key>CFBundleDevelopmentRegion</key>\
            <string>English</string>\
            <key>CFBundleExecutable</key>\
            <string>${GAME_NAME}</string>\
            <key>CFBundleIconFile</key>\
            <string>${ICON}</string>\
            <key>CFBundleIdentifier</key>\
            <string>au.edu.swinburne.${GAME_NAME}</string>\
            <key>CFBundleInfoDictionaryVersion</key>\
            <string>6.0</string>\
            <key>CFBundleName</key>\
            <string>${GAME_NAME}</string>\
            <key>CFBundlePackageType</key>\
            <string>APPL</string>\
            <key>CFBundleSignature</key>\
            <string>SWIN</string>\
            <key>CFBundleVersion</key>\
            <string>1.0</string>\
            <key>CSResourcesFileMapped</key>\
            <true/>\
    </dict>\
    </plist>" >> "${GAMEAPP_PATH}/Contents/Info.plist"

    echo "APPLSWIN" >> "${GAMEAPP_PATH}/Contents/PkgInfo"

    RESOURCE_DIR="${GAMEAPP_PATH}/Contents/Resources"
}

doLinuxCompile()
{
    mkdir -p "${TMP_DIR}"
    for file in `find ${LIBSRC_DIR} | grep [.]cpp$`; do
        name=${file##*/} # ## = delete longest match for */... ie all but file name
        name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp
        doCompile "${file}" "${name}" "${TMP_DIR}/${name}.o" ""
    done
    
    doCompileGameMain ""
    
    #Assemble all of the .s files
    echo "  ... Creating game"
    
    ${GCC_BIN} -L${LIB_DIR} -o "${OUT_DIR}/${GAME_NAME}" `find ${TMP_DIR} -maxdepth 1 -name \*.o` -lsgsdk
    if [ $? != 0 ]; then echo "Error creating game"; cat ${LOG_FILE}; exit 1; fi
}

doLinuxPackage()
{
    RESOURCE_DIR="${FULL_OUT_DIR}/Resources"
}

doWindowsCompile()
{
    mkdir -p "${TMP_DIR}"
    for file in `find ${LIBSRC_DIR} | grep [.]cpp$`; do
        name=${file##*/} # ## = delete longest match for */... ie all but file name
        name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp
        doCompile "${file}" "${name}" "${TMP_DIR}/${name}.o" ""
    done

    for file in `find ${SRC_DIR} | grep [.]cpp$`; do
	name=${file##*/} # ## = delete longest match for */... ie all but file name
	name=${name%%.cpp} # %% = delete longest match from back, i.e. extract .cpp

	if [ "${name}" != "${GAMEMAIN}" ]; then
	    doCompile "${file}" "${name}" "${TMP_DIR}/${name}.o" ""
	fi
    done
    
    doCompileGameMain ""
    
    #Assemble all of the .s files
    echo "  ... Creating game"
    ${GCC_BIN} -L${LIB_DIR} -static-libstdc++ -static-libgcc -march=i386 -o "${OUT_DIR}/${GAME_NAME}.exe" `find ${TMP_DIR} -maxdepth 1 -name \*.o` -lsgsdk
    if [ $? != 0 ]; then echo "Error creating game"; cat ${LOG_FILE}; exit 1; fi
}

doWindowsPackage()
{
    echo "  ... Copying libraries"
    cp -p -f "${LIB_DIR}"/*.dll "${OUT_DIR}"
    
    RESOURCE_DIR=${FULL_OUT_DIR}/Resources
}

copyWithoutSVN()
{
    FROM_DIR=$1
    TO_DIR=$2
    
    cd "${FROM_DIR}"
    
    # Create directory structure
    find . -mindepth 1 -type d ! -path \*.svn\* -exec sh -c "if [ ! -d '${TO_DIR}/{}' ]; then mkdir -p '${TO_DIR}/{}' ; fi" \;
    # Copy files and links
    find . ! -path \*.svn\* ! -name \*.DS_Store ! -type d -exec cp -R -p {} "${TO_DIR}/{}"  \;
}

#
# Copy Resources from standard location to $RESOURCE_DIR
#
doCopyResources()
{
    echo "  ... Copying Resources into $GAME_NAME"
    
    copyWithoutSVN "${APP_PATH}/Resources" "${RESOURCE_DIR}"
}

#
# Locate GameMain.pas
#
locateGameMain()
{
  cd "${SRC_DIR}"
  fileList=$(find "." -maxdepth 1 -type f -name \*.cpp)
  FILE_COUNT=$(echo "$fileList" | tr " " "\n" | wc -l)
  
  if [ ${FILE_COUNT} = 1 ]; then
    GAME_MAIN=${fileList[0]}
  else
    echo "Select the file to compile for your game"
    PS3="File number: "
  
    select fileName in $fileList; do
        if [ -n "$fileName" ]; then
            GAME_MAIN=${fileName}
        fi
      
        break
    done
  fi
  
  cd "${FULL_APP_PATH}"
  
  if [ ! -f "${SRC_DIR}/${GAME_MAIN}" ]; then
    echo "Cannot find file to compile, was looking for ${GAME_MAIN}"
    exit -1
  fi
}

GAME_MAIN="main.cpp"

if [ $CLEAN = "N" ]
then
    if [ ! -d "${OUT_DIR}" ]
    then
        mkdir -p "${OUT_DIR}"
    fi
    
    echo "--------------------------------------------------"
    echo "          Creating $GAME_NAME"
    echo "          for $OS"
    echo "--------------------------------------------------"
    echo "  Running script from $FULL_APP_PATH"
    echo "  Saving output to $OUT_DIR"
    echo "  Compiler flags ${SG_INC} ${C_FLAGS}"
    echo "--------------------------------------------------"
    echo "  ... Creating ${GAME_NAME}"

    
    if [ "$OS" = "$MAC" ]; then
        HAS_PPC=false
        HAS_i386=false
        HAS_LEOPARD_SDK=false
        OS_VER=`sw_vers -productVersion | awk -F . '{print $1"."$2}'`
        
        if [ -f /usr/libexec/as/ppc/as ]; then
            HAS_PPC=true
        fi
        
        if [ -f /usr/libexec/as/i386/as ]; then
            HAS_i386=true
        fi
        
        if [ -d /Developer/SDKs/MacOSX10.5.sdk ]; then
            HAS_LEOPARD_SDK=true
        fi
        
        if [ $OS_VER = '10.5' ]; then
            HAS_LEOPARD_SDK=true
        fi
        
        if [[ $HAS_i386 = true && $HAS_PPC = true && $HAS_LEOPARD_SDK ]]; then
            echo "  ... Building Universal Binary"
            
            if [ $OS_VER = '10.5' ]; then
                doMacCompile "i386" ""
                doMacCompile "ppc" ""
            else
                doMacCompile "i386" "-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
                doMacCompile "ppc" "-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
            fi
            
            doLipo "i386" "ppc"
        else
            doBasicMacCompile
        fi
        
        doMacPackage
    elif [ "$OS" = "$LIN" ]; then
        doLinuxCompile
        doLinuxPackage
    else #Windows
        doWindowsCompile
        doWindowsPackage
    fi
    
    doCopyResources
else
    CleanTmp
    rm -rf "${APP_PATH}/bin"
    mkdir "${APP_PATH}/bin"
    echo    ... Cleaned
fi

#remove temp files on success
rm -f ${LOG_FILE} 2>> /dev/null

echo "  Finished"
echo "--------------------------------------------------"
