# Banner
Write-Host "------------------------------------------------"
Write-Host ""
Write-Host ""
Write-Host "Welcome! It is CodeTest installer for Windows."
Write-Host ""
Write-Host "You must have administrator rights for installation" -ForegroundColor Green
Write-Host ""
Write-Host "This programs will be installed, if they are not installed yet:"
Write-Host "    - choco / Chocolatey"
Write-Host "    - gcc/g++ / GNU Compiler Collection"
Write-Host "    - cmake / CMake"
Write-Host "    - ninja / Ninja-build"
Write-Host ""
Write-Host ""
Write-Host "------------------------------------------------"
Write-Host ""

# Admin check

if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "You must have administrator rights for installation." -ForegroundColor Red
    Exit 1
}


$script:paths_to_utilities = [System.Collections.Hashtable]@{}
$script:uninstalled_utilities = [System.Collections.ArrayList]@()

function CheckInstalled {
    param ($utility)
    $programs = @("ProgramData", "Program Files", "Users")
    foreach ($dir in $programs) {
        $path = (where.exe /R "C:\$dir" $utility) -split " C:" 2>$null
        if (-not $path) { continue }
        foreach ($p in $path) {
            if (($path | Select-String -Pattern "[$util].exe")) { $script:paths_to_utilities.Add($utility, $p); return 1 }
        }
    }
    $script:uninstalled_utilities.Add($path)
    return 0
}

# Check installed utilities

Write-Host "Started checking installed utilities..."
Write-Host ""

$script:check_installation = @("choco", "gcc", "g++", "cmake", "ninja", "make")

foreach ($util in $script:check_installation) {
    Write-Host "Checking $util" -ForegroundColor Cyan
    $is_installed = CheckInstalled $util
    if ($is_installed -eq 1) {
        Write-Host "$util installed" -ForegroundColor Green
    } else {
        Write-Host "$util is not installed" -ForegroundColor Red
    }
    Write-Host "-----------------"
}


if ($script:uninstalled_utilities.Count -gt 0) {
    Write-Host ""
    $answer = Read-Host "Install it? CodeTest would not work without them. [Y/N]"

    if ($answer -eq "Y" ) {
        foreach ($util in $script:uninstalled_utilities) {
            Write-Host ""
            Write-Host "Installing $util" -ForegroundColor Blue
            Write-Host ""


            switch -Regex ($util) {
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
                        CheckInstalled "choco" > $null
                        break
                    } catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "cmake" {
                    try { choco install cmake --yes; CheckInstalled "cmake" > $null;  }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "make" {
                    try { choco install make --yes; CheckInstalled "make" > $null}
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "ninja" {
                    try { choco install ninja --yes; CheckInstalled "ninja" > $null }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
                "g" {
                    try {

                        if (Get-Module -ListAvailable -Name 7Zip4PowerShell) {
                            Write-Host "7Z powershell module installed" -ForegroundColor Blue
                        } else {
                            Write-Host ""
                            $ans = Read-Host "7Z powershell module is not intalled. Install it? [Y/N]"
                            if ($ans -eq "Y") {
                                [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
                                Install-PackageProvider -Name NuGet -MinimumVersion 2.8.5.201 -Force
                                Set-PSRepository -Name 'PSGallery' -SourceLocation "https://www.powershellgallery.com/api/v2" -InstallationPolicy Trusted
                                Install-Module -Name 7Zip4PowerShell -Force
                            } else {
                                Write-Host "Installation canceled by user." -ForegroundColor Red
                            }
                        }
                        $ProgressPreference = "SilentlyContinue"
                        Invoke-WebRequest "http://www.1.m-teacher.ru/files/gcc-14.2.0-no-debug.7z" -OutFile ".\gcc-14.2.0-no-debug.7z"
                        Expand-7Zip -ArchiveFileName ".\gcc-14.2.0-no-debug.7z" -TargetPath ".\gcc-14.2.0-no-debug"
                        $ProgressPreference = "Continue"
                        break
                    }
                    catch { Write-Host "Error: $_" -ForegroundColor Red }
                }
            }
            Write-Host ""
            Write-Host "$util installed" -ForegroundColor Green
            Write-Host "------------------------"
        }
    } else {
        Write-Host "Installation canceled by user" -ForegroundColor Red
        Exit 1
    }
}

Write-Host "Started building..." -ForegroundColor Blue

$env:CC = $($script:paths_to_utilities["gcc"].ToString()).ToString().Trim()
$env:CXX = $($script:paths_to_utilities["g++"].ToString()).ToString().Trim()

& $script:paths_to_utilities["cmake"] -G "Ninja" .
& $script:paths_to_utilities["cmake"] --build .


Write-Host ""
Write-Host "Installation complete." -ForegroundColor Blue
Write-Host "Usage .\codetest.exe --help"