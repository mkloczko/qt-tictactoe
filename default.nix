{ stdenv
, lib
, cmake
, ninja
, gcc
, qt6
, gtest
}:

stdenv.mkDerivation rec {
  pname = "qt-tictactoe";
  version = "0.1.0";
  
  src = ./.;

  nativeBuildInputs = [ qt6.wrapQtAppsHook cmake ninja gcc ];
  buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qtlocation gtest ];

  cmakeFlags = [
    "-DENABLE_TESTING=OFF"
    "-DENABLE_INSTALL=ON"
  ];

  meta = with lib; {
    homepage = "https://github.com/mkloczko/qt-tictactoe";
    description = ''
        A small sample app implementing a tic-tac-toe game in Qt.
    '';
    licencse = licenses.mit;
    platforms = with platforms; linux;
  };
}
