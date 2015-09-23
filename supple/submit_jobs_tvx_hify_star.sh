#!/usr/bin/env bash
#
# Usage:
#
# $ submit_jobs_stihify.sh
#


: ${TVX_SOURCE_DIR:=${HOME}/travex}
: ${TVX_BUILD_DIR:=${TVX_SOURCE_DIR}/build2}
: ${TVX_INSTALL_PREFIX:=${HOME}/tvx_install}
: ${OUT_DIR:=${HOME}/tmp_hify_out}


echo "The following variables will be used:"
echo -e "\t TVX_SOURCE_DIR:     $TVX_SOURCE_DIR"
echo -e "\t TVX_BUILD_DIR:      $TVX_BUILD_DIR"
echo -e "\t TVX_INSTALL_PREFIX: $TVX_INSTALL_PREFIX"
echo -e "\t OUT_DIR:            $OUT_DIR"


TVX_DEACT_DET_LAYER="pxl_1"
export TVX_INSTALL_DIR="${TVX_INSTALL_PREFIX}_$TVX_DEACT_DET_LAYER"

# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG


cd $TVX_BUILD_DIR && pwd
cmake -D CMAKE_INSTALL_PREFIX=$TVX_INSTALL_DIR -D CMAKE_CXX_FLAGS="-m32" -D BOOST_ROOT=$OPTSTAR ${TVX_SOURCE_DIR}
make -j8 install

cp $TVX_SOURCE_DIR/y2014a.root $TVX_INSTALL_DIR/
cp $TVX_SOURCE_DIR/supple/run_tvx_hify_star.sh $TVX_INSTALL_DIR/
mkdir -p /tmp/my_sums/
mkdir -p $OUT_DIR

echo
echo "Submitting job for $TVX_DEACT_DET_LAYER"
star-submit-template -template $TVX_SOURCE_DIR/supple/job_template_tvx_hify_star.xml \
   -entities TVX_SOURCE_DIR=$TVX_SOURCE_DIR,TVX_INSTALL_DIR=$TVX_INSTALL_DIR,TVX_DEACT_DET_LAYER=$TVX_DEACT_DET_LAYER,OUT_DIR=$OUT_DIR
echo