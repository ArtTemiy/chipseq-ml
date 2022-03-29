# DESCRIPTION AND USAGE:
# Runs script with creation of output log and other pretty things
# sh ./run_script <path_to_script>

# for logs
mkdir -p /tmp/bioinf/scripts
LOG_OUT_FILE=/tmp/bioinf/scripts/$(basename $1 .sh)_stdout.txt
LOG_ERR_FILE=/tmp/bioinf/scripts/$(basename $1 .sh)_stderr.txt
touch $LOG_OUT_FILE
touch $LOG_ERR_FILE
echo log output file: $LOG_OUT_FILE
echo log output error file: $LOG_ERR_FILE

sh $1 >> $LOG_OUT_FILE 2>> $LOG_ERR_FILE
