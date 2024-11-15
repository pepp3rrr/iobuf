{
  description = "Cringe dev environment (it's C dude)";

  inputs = { };

  outputs = { self, nixpkgs } @ inputs:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.${system}.default = with pkgs; mkShell rec {
        packages = [
          gdb
          lldb
          bear
          gnumake
          clang-tools
          nixpkgs-fmt
        ];
        nativeBuildInputs = [
          pkg-config
          gcc
        ];
        buildInputs = [
          glibc
        ];
        LD_LIBRARY_PATH = lib.makeLibraryPath buildInputs;
      };
    };
}
