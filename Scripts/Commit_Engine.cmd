rem https://stackoverflow.com/questions/18972156/how-to-pass-input-to-exe-in-batch-file
rem QwerkE_Engine

cd ../

rem grab any new files or changes. Maybe have merge issues here
git pull
pause

rem commit local changes to local repo
set /p message=Commit message "..."?
git add .
git commit -m "%message%"

rem push local commit up to master
git push
pause