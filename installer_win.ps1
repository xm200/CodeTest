Write-Host "------------------------------------------------"
Write-Host ""
Write-Host ""
Write-Host "Здравсвтвуйте! Это скрипт-установщик CodeTest для Windows."
Write-Host ""
Write-Host "Для установки нужны права администратора"
Write-Host ""
Write-Host "Необходимо установить:"
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
    Write-Host "Запустите установщик с правами администратора."
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
    Write-Host "Chocolatey установлен" -ForegroundColor Green
} else {
    Write-Host "Chocolatey не установлен" -ForegroundColor Red
}

if (CheckInstalled "cmake") {
    Write-Host "CMake установлен" -ForegroundColor Green
} else {
    Write-Host "Cmake не установлен" -ForegroundColor Red
}

if (CheckInstalled "make") {
    Write-Host "make установлен" -ForegroundColor Green
} else {
    Write-Host "make не установлен" -ForegroundColor Red
}

if (CheckInstalled "ninja") {
    Write-Host "Ninja-build установлен"-ForegroundColor Green
} else {
    Write-Host "Ninja-build не установлен" -ForegroundColor Red
}

if (CheckInstalled "gcc") {
    Write-Host "gcc установлен" -ForegroundColor Green
} else {
    Write-Host "gcc не установлен" -ForegroundColor Red
}

if (CheckInstalled "g++") {
    Write-Host "g++ установлен" -ForegroundColor Green
} else {
    Write-Host "g++ не установлен" -ForegroundColor Red
}


if ($script:uninstalled_utilities.Count -gt 0) {
    Write-Host ""
    $answer = Read-Host "Установить их? CodeTest не может быть собран без них. [Д/Н]"

    if ($answer -eq "Д") {
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
                    } catch { Write-Host "Ошибка: $_" -ForegroundColor Red }
                }
                "cmake" {
                    try { choco install cmake }
                    catch { Write-Host "Ошибка: $_" -ForegroundColor Red }
                }
                "make" {
                    try { choco install make }
                    catch { Write-Host "Ошибка: $_" -ForegroundColor Red }
                }
                "ninja" {
                    try { choco install ninja }
                    catch { Write-Host "Ошибка: $_" -ForegroundColor Red }
                }
            }
        }
    }
}
