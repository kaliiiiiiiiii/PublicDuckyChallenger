# add audio interface
# https://stackoverflow.com/a/31751275/20443541
Add-Type -TypeDefinition @'
using System.Runtime.InteropServices;
[Guid("5CDF2C82-841E-4546-9722-0CF74078229A"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IAudioEndpointVolume
{
    // f(), g(), ... are unused COM method slots. Define these if you care
    int f(); int g(); int h(); int i();
    int SetMasterVolumeLevelScalar(float fLevel, System.Guid pguidEventContext);
    int j();
    int GetMasterVolumeLevelScalar(out float pfLevel);
    int k(); int l(); int m(); int n();
    int SetMute([MarshalAs(UnmanagedType.Bool)] bool bMute, System.Guid pguidEventContext);
    int GetMute(out bool pbMute);
}
[Guid("D666063F-1587-4E43-81F1-B948E807363F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IMMDevice
{
    int Activate(ref System.Guid id, int clsCtx, int activationParams, out IAudioEndpointVolume aev);
}
[Guid("A95664D2-9614-4F35-A746-DE8DB63617E6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface IMMDeviceEnumerator
{
    int f(); // Unused
    int GetDefaultAudioEndpoint(int dataFlow, int role, out IMMDevice endpoint);
}
[ComImport, Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")] class MMDeviceEnumeratorComObject { }
public class Audio
{
    static IAudioEndpointVolume Vol()
    {
        var enumerator = new MMDeviceEnumeratorComObject() as IMMDeviceEnumerator;
        IMMDevice dev = null;
        Marshal.ThrowExceptionForHR(enumerator.GetDefaultAudioEndpoint(/*eRender*/ 0, /*eMultimedia*/ 1, out dev));
        IAudioEndpointVolume epv = null;
        var epvid = typeof(IAudioEndpointVolume).GUID;
        Marshal.ThrowExceptionForHR(dev.Activate(ref epvid, /*CLSCTX_ALL*/ 23, 0, out epv));
        return epv;
    }
    public static float Volume
    {
        get { float v = -1; Marshal.ThrowExceptionForHR(Vol().GetMasterVolumeLevelScalar(out v)); return v; }
        set { Marshal.ThrowExceptionForHR(Vol().SetMasterVolumeLevelScalar(value, System.Guid.Empty)); }
    }
    public static bool Mute
    {
        get { bool mute; Marshal.ThrowExceptionForHR(Vol().GetMute(out mute)); return mute; }
        set { Marshal.ThrowExceptionForHR(Vol().SetMute(value, System.Guid.Empty)); }
    }
}
'@

Add-Type -AssemblyName System.Windows.Forms

function StartEdge([string]$url, [int]$timeout){
    if($timeout -ne 0){Start-Sleep -Milliseconds $timeout}
    $process = Start-Process "msedge" -WindowStyle Maximized -PassThru -ArgumentList "--edge-kiosk-type=fullscreen", "--autoplay-policy=no-user-gesture-required","--start-fullscreen","--disable-infobars", "--no-first-run","--kiosk","--start-maximized", "--disable-pinch", "--overscroll-history-navigation=0", "--touch-events=disabled", $url
    return $process
}

function StartKiosk([string]$url, [int]$timeout){
    $orgininalVolume = [audio]::Volume
    $originalMuteSetting = [audio]::Mute
    $originalMousePosition = [System.Windows.Forms.Cursor]::Position

    $start = Get-Date
    $process = StartEdge $url 0
    while($start.AddSeconds($timeout) -gt (Get-Date)){# while not $timeout seconds over
        # yeet mouse out of view
        [System.Windows.Forms.Cursor]::Position = New-Object System.Drawing.Point(100, 100)


        # maximize volume & disable mute
        [audio]::Mute = $false
        [audio]::Volume = 1

        try{Stop-Process -Name explorer}catch{}

        if($process.HasExited){
            $process = StartEdge $url 1000
            $last_start = Get-Date
        }
        Start-Sleep -Milliseconds 5
    };

    # kill edge kiosk mode
    $process.Kill()
    # revert all settings
    Start-Process explorer
    [audio]::Volume = $orgininalVolume
    [audio]::Mute = $originalMuteSetting
    [System.Windows.Forms.Cursor]::Position = $originalMousePosition
}

$timeout = 60 * 3
# $url = "https://fakeupdate.net/win10ue/"
$url = "https://kaliiiiiiiiii.github.io/rickroll/"
StartKiosk  $url $timeout