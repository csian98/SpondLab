#/bin/bash
USER=$1

mkdir -m 777 -p pth .SESSA data .tmp/log .tmp/losses .tmp/dump .tmp/dump2 data shut
chown -R $USER .
chgrp -R $USER .
