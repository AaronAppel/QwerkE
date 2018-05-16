rem https://stackoverflow.com/questions/18972156/how-to-pass-input-to-exe-in-batch-file
cd QwerkE_framework
set /p message=Commit message "..."?
git add .
git commit -m %message%
git push
pause