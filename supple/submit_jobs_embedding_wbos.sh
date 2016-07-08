#!/usr/bin/env sh
#
# See project's README.md for details
#

sample="Wplus_enu"
#sample=Wplus_taunu
#sample=Wminus_enu
#sample=Wminus_taunu
#sample=Z_eplus_eminus_inter
#sample=QCD

codePath="$HOME/star-vertex-eval/"
outPath="$HOME/scratch/wbos_embed/"
BFC_MIXER_MACRO="$codePath/supple/bfcMixer_pythia.C"
VERTEX_GEN_MACRO="$codePath/ext/star-macros/macros/embedding/GenerateVertex.C"
#VERTEX_PARAMS_DB_FILE="$codePath/ext/star-db-calibrations/tracker/PrimaryVertexCuts.20121221.000000.C"
STARSIM_KUMAC="$codePath/supple/simWRequest.kumac"


echo -e "Named arguments and their values:"
echo -e "\t sample: $sample"
echo -e "\t codePath: $codePath"
echo -e "\t outPath: $outPath"
echo -e "\t BFC_MIXER_MACRO: $BFC_MIXER_MACRO"
echo -e "\t VERTEX_GEN_MACRO: $VERTEX_GEN_MACRO"
echo -e "\t VERTEX_PARAMS_DB_FILE: \"$VERTEX_PARAMS_DB_FILE\""
echo -e "\t STARSIM_KUMAC: $STARSIM_KUMAC"


#create output directories
mkdir -p $outPath/root/
mkdir -p $outPath/log/
mkdir -p $outPath/fzd/
mkdir -p $outPath/sums/
mkdir -p ./StarDb/Calibrations/tracker/

cp $BFC_MIXER_MACRO ./
cp $VERTEX_GEN_MACRO ./
cp $STARSIM_KUMAC ./

if [ -n "${VERTEX_PARAMS_DB_FILE}" ]; then
   cp $VERTEX_PARAMS_DB_FILE ./StarDb/Calibrations/tracker/
else
   echo -e "\nNOTICE: No \$VERTEX_PARAMS_DB_FILE was provided"
fi


index=2000
norm=1.0
#norm=1.0 for 110K events (W+)
#norm=0.32 for 35K events (W-)
#norm=0.23 for 25K events (Z/gamma*)


IFS=\  #set space as delimeter
while read daqFile runNumber nEvents
do

  nEventsScaled=$(expr $nEvents*$norm |tr -d $'\r'| bc)
  nEventsPretty=`echo $nEventsScaled | awk '{printf("%d\n",$1+0.5)}'`
  nEvents=$nEventsPretty


  #set queue length by # of target events
  filesPerHour=0.01 
  if [ $nEvents -lt 50 ] 
  then
    filesPerHour=0.21
  fi
  if [ $nEvents -lt 10 ]
  then
    filesPerHour=0.34
  fi


  echo -e "\nProcessing new line from input file list"
  echo -e "\t index: $index"
  echo -e "\t nEvents [nEventsScaled, nEventsPretty]: $nEvents [$nEventsScaled, $nEventsPretty]"
  echo -e "\t daqFile: $daqFile"
  echo -e "\t runNumber: $runNumber"
  echo -e "\t filesPerHour: $filesPerHour"

  star-submit-template -template $codePath/supple/job_template_embedding_wbos.xml -entities  nEvents=$nEvents,outPath=$outPath,codePath=$codePath,daqFile=$daqFile,runNumber=$runNumber,sample=$sample,nIndex=$index,filesPerHour=$filesPerHour

  index=`expr $index \+ 1`

done <  $codePath/supple/filelist_zerobias.txt