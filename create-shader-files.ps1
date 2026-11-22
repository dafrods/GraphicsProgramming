[CmdletBinding(SupportsShouldProcess)]
param(
    [Parameter(Mandatory)]
    [string]$FileName
)

if ($Debug){
    $DebugPreference = "Continue"
}

$SHADER_PATH = "$($PSScriptRoot)\assets\shaders"

Write-Debug $SHADER_PATH

Copy-Item -force "$SHADER_PATH\base.vert" "$SHADER_PATH\b_$FileName.vert"
Copy-Item -force "$SHADER_PATH\base.frag" "$SHADER_PATH\b_$FileName.frag"
Copy-Item -force "$SHADER_PATH\light.vert" "$SHADER_PATH\l_$FileName.vert"
Copy-Item -force "$SHADER_PATH\light.frag" "$SHADER_PATH\l_$FileName.frag"


# End
if ($Debug){
    $DebugPreference = "SilentlyContinue"
}















