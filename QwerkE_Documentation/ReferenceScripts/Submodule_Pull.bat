rem pulls down updated versions of all submodul projects

rem git submodule foreach git pull origin master --merge // recursive. use if more than 1 submodule maybe

cd QwerkE_Framework
git checkout master && git pull
cd..
