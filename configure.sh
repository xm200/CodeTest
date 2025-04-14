sub_install() {
  local tmp_path
  tmp_path=$(which "$1")
  if [[ $tmp_path == "" ]]
  then
    echo -n "$1 not found. Do you want to install it manually? ($1 license places in \"third party licenses\" dir) [Y/n]: "
    ans=""
    read -r ans
    if [[ $ans == "Y" || $ans == "" || $ans == "y" ]]
    then
      sudo apt update
      sudo apt install "$2"
    else
      echo "Without dependencies this project can not be compiled. Aborting"
      exit 0
    fi
  fi
  tmp_path=$(which "$1")
  if [[ $tmp_path == "" ]]
    then
      echo "Something wrong"
      exit 1
  fi
  echo "$1 path = $tmp_path"
}

sub_install "make" "build-essential"
sub_install "cmake" "cmake"
sub_install "gcc" "gcc"
sub_install "g++" "g++"

cmake .