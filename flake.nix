{
  description = "Best game";

  inputs = {};

  outputs = {self, nixpkgs} @ inputs: let
    system ="x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = with pkgs; mkShell rec {
      packages = [
        clang-tools
        lldb
        cmake
        cmake-language-server
        gdb
      ];
    };
  };
}
