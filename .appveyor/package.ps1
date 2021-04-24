$ErrorActionPreference = "Stop"

if ($env:TS -eq '0') {
    $ts_part = 'nts'
} else {
    $ts_part = 'ts';
}

if ($env:APPVEYOR_REPO_TAG -eq "true") {
    $bname = "php_simple_kafka_client-$env:APPVEYOR_REPO_TAG_NAME-$env:PHP_VER-$ts_part-$env:VC-$env:ARCH"
} else {
    $bname = "php_simple_kafka_client-$($env:APPVEYOR_REPO_COMMIT.substring(0, 8))-$env:PHP_VER-$ts_part-$env:VC-$env:ARCH"
}
$zip_bname = "$bname.zip"

$dir = 'C:\projects\php_simple_kafka_client\';
if ($env:ARCH -eq 'x64') {
    $dir += 'x64\'
}
$dir += 'Release'
if ($env:TS -eq '1') {
    $dir += '_TS'
}

$files = @(
    "$dir\php_simple_kafka_client.dll",
    "$dir\php_simple_kafka_client.pdb",
    "C:\projects\php_simple_kafka_client\LICENSE",
    "C:\projects\php_simple_kafka_client\README.md",
    "C:\build-cache\deps\bin\librdkafka.dll",
    "C:\build-cache\deps\bin\librdkafka.pdb",
    "C:\build-cache\deps\LICENSE.LIBRDKAFKA"
)
Compress-Archive $files "C:\$zip_bname"
Push-AppveyorArtifact "C:\$zip_bname"
