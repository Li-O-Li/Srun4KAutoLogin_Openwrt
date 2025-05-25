# if can't understand their meaning, just keep default.
SRUN_INSTALL_BIN_FOLDER=/usr/bin
SRUN_INSTALL_SHELL_FOLDER=/usr/scripts
SRUN_INSTALL_BIN_NAME=srunRequestBodyGeneration
SRUN_INSTALL_SHELL_NAME=autoLogin.sh
SRUN_LOG_PATH=/tmp/login.log

# modify the boot script
sed -i "s|\[EXECUTABLE_PATH\]|$SRUN_INSTALL_BIN_FOLDER/$SRUN_INSTALL_BIN_NAME|g" ./shell/autoLogin.sh
sed -i "s|\[LOG_PATH\]|$SRUN_LOG_PATH|g" ./shell/autoLogin.sh
sed -i "s|\[LOGIN_SCRIPT_PATH\]|$SRUN_INSTALL_SHELL_FOLDER/$SRUN_INSTALL_SHELL_NAME|g" ./shell/srun_login

# Check if the folders exist, if not, create them
[ ! -d "$SRUN_INSTALL_BIN_FOLDER" ] && mkdir -p "$SRUN_INSTALL_BIN_FOLDER"
[ ! -d "$SRUN_INSTALL_SHELL_FOLDER" ] && mkdir -p "$SRUN_INSTALL_SHELL_FOLDER"

# install files and set permissions
cp ./bin/srunRequestBodyGeneration "$SRUN_INSTALL_BIN_FOLDER/$SRUN_INSTALL_BIN_NAME"
cp ./shell/autoLogin.sh "$SRUN_INSTALL_SHELL_FOLDER/$SRUN_INSTALL_SHELL_NAME"
cp ./shell/srun_login /etc/init.d/srun_login
chmod 755 "$SRUN_INSTALL_BIN_FOLDER/$SRUN_INSTALL_BIN_NAME"
chmod 755 "$SRUN_INSTALL_SHELL_FOLDER/$SRUN_INSTALL_SHELL_NAME"
chmod 755 /etc/init.d/srun_login