{pkgs ? import <nixpkgs> {}}:
with pkgs;
  mkShell rec {
    packages = with pkgs; [
      helix
      clang-tools
      lldb
      cmake
      cmake-language-server
      gdb
    ];
  }
