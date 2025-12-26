#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash html_to_odf main.o simple.html simple.odf
qmake -makefile html_to_odf.pro 2>&1 | tee go.out
gmake 2>&1                           | tee -a go.out
ldd html_to_odf 2>&1                 | tee -a go.out

cat > simple.html << EOF
<!DOCTYPE html>
<html>
<head>
  <title>My Simple Page</title>
</head>
<body>
  <h1>Welcome to My Simple Page</h1>
  <p>This is a paragraph of text.</p>
</body>
</html>
EOF
./html_to_odf simple.html simple.odf 2>&1 | tee -a go.out
cat -n simple.odf 2>&1                    | tee -a go.out

rm *~ .qmake.stash main.o
