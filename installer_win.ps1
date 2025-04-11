Write-Host "------------------------------------------------"
Write-Host ""
Write-Host ""
Write-Host "Hello! It is installer CodeTest for Windows."
Write-Host ""
Write-Host "You need to have administrator rules for install"
Write-Host ""
Write-Host "This programs will be installed, if they are not installed yet:"
Write-Host "    - choco / Chocolatey"
Write-Host "    - gcc/g++ / GNU Compiler Collection"
Write-Host "    - cmake / CMake"
Write-Host "    - ninja / Ninja-build"
Write-Host "    - make"
Write-Host ""
Write-Host ""
Write-Host "------------------------------------------------"
Write-Host ""
Write-Host ""


if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "You need to have administrator rules for install"
    Exit 1
}


$script:paths_to_utilities = [System.Collections.ArrayList]@()
$script:uninstalled_utilities = [System.Collections.ArrayList]@()

function CheckInstalled {
    param ($utility)
    try {
        $temp_path = where.exe /R "C:\Users" $utility 2>$null
        $paths = $temp_path -split " C:"
        if ($paths) {
            foreach ($path in $paths) {
                $script:paths_to_utilities.Add($path)
            }
            return 1
        } else {
            $script:uninstalled_utilities.Add($path)
        }
    }
    catch {
        return 0
    }
}


if (CheckInstalled "choco") {
    Write-Host "Chocolatey is installed" -ForegroundColor Green
} else {
    Write-Host "Chocolatey is not installed" -ForegroundColor Red
}

if (CheckInstalled "cmake") {
    Write-Host "CMake is installed" -ForegroundColor Green
} else {
    Write-Host "Cmake is not installed" -ForegroundColor Red
}

if (CheckInstalled "make") {
    Write-Host "make is installed" -ForegroundColor Green
} else {
    Write-Host "make is not installed" -ForegroundColor Red
}

if (CheckInstalled "ninja") {
    Write-Host "Ninja-build is installed"-ForegroundColor Green
} else {
    Write-Host "Ninja-build is not installed" -ForegroundColor Red
}

if (CheckInstalled "gcc") {
    Write-Host "gcc is installed" -ForegroundColor Green
} else {
    Write-Host "gcc is not installed" -ForegroundColor Red
}

if (CheckInstalled "g++") {
    Write-Host "g++ is installed" -ForegroundColor Green
} else {
    Write-Host "g++ is not installed" -ForegroundColor Red
}


if ($script:uninstalled_utilities.Count -gt 0) {
    Write-Host ""
    $answer = Read-Host "Install it? [Y/N]"

    if ($answer -eq "Y" ) {
        foreach ($util in $script:uninstalled_utilities) {
            switch ($util) {
                "choco" {
                    try {
                        Set-ExecutionPolicy Bypass -Scope Process -Force
                        [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
                        iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
                        foreach($level in "Machine","User") {
                            [Environment]::GetEnvironmentVariables($level).GetEnumerator() | ForEach-Object {
                                $env:Path += ";$($_.Value)"
                            }
                        }
                    } catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "cmake" {
                    try { choco install cmake }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "make" {
                    try { choco install make }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "ninja" {
                    try { choco install ninja }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
            }
        }
    }
}
