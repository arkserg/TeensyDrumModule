
namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums
{
    public enum CommandType : byte
    {
        Ping                = 1,
        GetAllDrums         = 2,
        SetDrumParameters   = 3,
        SaveSettings        = 4,
        ReloadSettings      = 5,
        EnableDrum          = 6,
        DisableDrum         = 7,
        //Info                = 9,
    }
}