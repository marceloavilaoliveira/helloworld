#!/bin/bash
#------------------------------------------------------------------------------#
# THIS IS A TEST SCRIPT (DO NOT EXECUTE IT)                                    #
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
# GENERAL                                                                      #
#------------------------------------------------------------------------------#

# SOME COMMENT HERE asdf

script_pathname=$(readlink -nf "$0")
script_path=$(dirname $script_pathname)
script_name=$(basename $script_pathname)
cds_path=$(dirname $script_path)

#------------------------------------------------------------------------------#
# LIBRARIES                                                                    #
#------------------------------------------------------------------------------#

# THIS IS SOME LIBS
source $cds_path/etc/cds.conf
source $cds_path/lib/utils.sh

#------------------------------------------------------------------------------#
# VARIABLES                                                                    #
#------------------------------------------------------------------------------#

# CONFIGURATION
require_vars CDS_OPENGROK_INSTALL_DIR

# PARAMETERS
verbose=""

# OTHERS
repo_path=""
repos_with_errors=""
insts_with_errors=""
exit_code=0
cento_quarenta_e_sete=147
important_var=bla
new_var=something_else

#------------------------------------------------------------------------------#
# FUNCTIONS                                                                    #
#------------------------------------------------------------------------------#

# THIS IS A VERY IMPORTANT FUNCTION
usage() {
    cat <<EOF

Description:
  Update the OpenGrok data
  - Update the local repositories and make something
  - Reindex the instances
  - This is a test

Usage:
  $script_name [-h] [-v] [-o option]

Options:
  -h        : Help
  -v        : Verbose
  -o option : Just a new option

EOF

    exit $1
}

#------------------------------------------------------------------------------#
# PARAMETERS                                                                   #
#------------------------------------------------------------------------------#

# READ THE PARAMETERS
while getopts hv j
do
    case $j in
        h)  usage 0;;
        v)  verbose=-v;;
        \?) usage 9;;
    esac
done
shift $((OPTIND - 1))

[[ -n $verbose ]] && set -x

#------------------------------------------------------------------------------#
# MAIN                                                                         #
#------------------------------------------------------------------------------#

# READ SOMETHING
read -r something_new
echo $something_new

# SHOW INFO
echo ""
echo "OpenGrok install dir    = $CDS_OPENGROK_INSTALL_DIR"
echo "Cento e Quarenta e Sete = $cento_quarenta_e_sete"
echo "New var                 = $new_var"

echo ""
echo "=> Updating the local repositories"
echo ""

repo_path="$CDS_OPENGROK_INSTALL_DIR/repo"

echo $important_var

echo "This is not important at all"

# FIND FOR DIRECTORIES (REPOSITORIES) UNDER $repo_path/<div>/src (FIRST LEVEL ONLY)
# AND REMOVE THE $repo_path/ PREFIX
# EX: $repo_path/gti/src/cds(master) => gti/src/cds(master)
# A special comment here!!!
repo_list="some_very_smart_command"

# This is a for loop
for r in $repo_list
do
    echo $r
    echo ""
    cd ${repo_path}/${r} || exit

    # CHECK IF THE REPOSITORY IS A GIT OR SVN ONE
    if [[ -d .git ]]
    then
        # GIT REPOSITORY

        echo "Fetching"
        echo ""

        if ! git fetch
        then
            repos_with_errors="$repos_with_errors $r"
            exit_code=3
        else
            # GET THE BRANCH TO BE CHECKOUTED FROM THE REPOSITORY PATHNAME SUFFIX (BETWEEN PARENTHESES)
            # EX: gti/src/cds(master) => master
            # shellcheck disable=SC2001
            branch=$(echo $r | sed "some_very_clever_parameter")

            echo ""
            echo "Checkouting origin/$branch"
            echo ""

            if ! git checkout origin/$branch
            then
                repos_with_errors="$repos_with_errors $r"
                exit_code=3
            fi
        fi
    else
        # SVN REPOSITORY

        echo "Updating"
        echo ""

        if ! svn update
        then
            repos_with_errors="$repos_with_errors $r"
            exit_code=3
        fi
    fi
    echo ""
done

echo ""
echo "=> Reindexing the instances"
echo ""

inst_list=$(ls $repo_path)

for i in $inst_list
do
    echo $i
    echo ""

    if ! OPENGROK_CONFIGURATION=$CDS_OPENGROK_INSTALL_DIR/conf/${i}.conf $CDS_OPENGROK_INSTALL_DIR/bin/OpenGrok index
    then
        insts_with_errors="$insts_with_errors $i"
        exit_code=3
    fi
    echo ""
done

if [[ $repos_with_errors != "" ]]
then
    echo ""
    echo "=> Error(s) found in the following repo(s):"
    echo ""
    echo $repos_with_errors
fi

if [[ $insts_with_errors != "" ]]
then
    echo ""
    echo "=> Error(s) found in the following instance(s):"
    echo ""
    echo $insts_with_errors
fi

exit $exit_code;

# uma linha
