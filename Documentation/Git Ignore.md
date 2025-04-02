#### To ignore changes to a specific file:
By name or relative path
git update-index --assume-unchanged "%~f0"

By absolute path
git update-index --assume-unchanged "%~f0"&

#### To resume tracking changes:
By name or relative path
git update-index --no-assume-unchanged "%~f0"