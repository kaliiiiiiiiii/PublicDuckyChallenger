function Exec {
    param (
        [string]$Url,
        [string[]]$Arguments = @()  # Optional array of arguments to pass to the executable
    )

    # Generate a temporary file path for the downloaded executable
    $tempFilePath = [System.IO.Path]::Combine([System.IO.Path]::GetTempPath(), [System.IO.Path]::GetRandomFileName() + ".exe")

    try {
        # Download the executable
        Write-Host "Downloading file from $Url..."
        Invoke-WebRequest -Uri $Url -OutFile $tempFilePath

        # Setup and start the process
        Write-Host "Starting process..."
        $process = New-Object System.Diagnostics.Process
        $process.StartInfo.FileName = $tempFilePath
        $process.StartInfo.Arguments = $Arguments -join " "  # Convert array to a space-separated string
        $process.StartInfo.UseShellExecute = $false
        $process.StartInfo.RedirectStandardOutput = $true
        $process.StartInfo.RedirectStandardError = $true
        $process.StartInfo.CreateNoWindow = $true

        # Event handlers for output and error data
        $process.add_OutputDataReceived({
            if ($_.Data) {
                [Console]::WriteLine($_.Data)
            }
        })

        $process.add_ErrorDataReceived({
            if ($_.Data) {
                [Console]::WriteLine($_.Data)
            }
        })

        $process.Start() | Out-Null
        $process.BeginOutputReadLine()
        $process.BeginErrorReadLine()
        $process.WaitForExit()
        [Console]::WriteLine("Process exited with ${$process.ExitCode}")
    } catch {
        [Console]::WriteLine("An error occurred: $_")
    } finally {
        if (Test-Path $tempFilePath) {
            Remove-Item $tempFilePath -Force
        }
    }
}

Exec -Url "https://www.python.org/ftp/python/3.12.4/python-3.12.4.exe" -Arguments @()
