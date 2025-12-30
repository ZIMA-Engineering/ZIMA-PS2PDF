{ pkgs ? (import <nixpkgs> {}) }:
let
  f =
    { stdenv, qt6, qtcreator, pkg-config, qmake, openssl, gdb, gcc }:
    let
      zimaQt = qt6.env "qt-zima-ps2pdf-${qt6.qtbase.version}" (with qt6; [
        qtbase qtdeclarative qttools
      ]);
    in stdenv.mkDerivation {
      name = "ZIMA-PS2PDF";
      src = ./.;
      nativeBuildInputs = [ zimaQt qtcreator pkg-config qmake gdb ];
      buildInputs = [ openssl ];
      shellHook = ''
        mkdir -p nix-build
        ln -sfn ${zimaQt} nix-build/qt
        ln -sfn ${gdb} nix-build/gdb
        ln -sfn ${gcc} nix-build/gcc
      '';
    };
in pkgs.qt6Packages.callPackage f {}

