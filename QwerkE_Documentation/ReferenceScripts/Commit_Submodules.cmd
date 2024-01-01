:: https://stackoverflow.com/questions/18972156/how-to-pass-input-to-exe-in-batch-file
cd QwerkE_Framework

:: grab any new files or changes. Maybe have merge issues here
git pull
pause

:: commit local changes to local repo
QwerkE_Framework
set /p message=Commit message "..."?
git add .
git commit -m %message%

:: push local commit up to master
git push
pause

cd ../
cd QwerkE_Documentation

:: grab any new files or changes. Maybe have merge issues here
git pull
pause

:: commit local changes to local repo
QwerkE_Documentation
set /p message=Commit message "..."?
git add .
git commit -m %message%

:: push local commit up to master
git push
pause