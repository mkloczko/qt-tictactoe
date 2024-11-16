{
  inputs = { nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05"; };

  outputs = inputs@{ flake-parts,... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"
      ];
      perSystem = {config, self', inputs', pkgs, system, ...}: 
        let concatStrings = pkgs.lib.strings.concatStrings;
            package = pkgs.callPackage ./default.nix {};
            dev_pkgs = with pkgs; [qtcreator ccls];
            qt_pkgs = with pkgs.qt6; [qtbase qtdeclarative qtlocation];
            isDir = pkgs.top-level.impure.isDir;
        in {
          packages.default = package;
          apps.default = {
              type = "app";
              program = "${self'.packages.default}/bin/tictactoe-app";
          };
          devShells.default =
            pkgs.mkShell rec { 
                nativeBuildInputs = dev_pkgs ++ package.nativeBuildInputs;
                inputsFrom = [self'.devShells];
                buildInputs = package.buildInputs;
                shellHook = ''
                '';
            };
        };
      

};

}
