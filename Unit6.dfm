object BootP: TBootP
  DisplayName = 'BootP'
  AfterInstall = ServiceAfterInstall
  OnStop = ServiceStop
  Height = 1440
  Width = 1920
  PixelsPerInch = 192
  object IdUDPServer1: TIdUDPServer
    Active = True
    Bindings = <>
    DefaultPort = 67
    ReuseSocket = rsTrue
    OnUDPRead = IdUDPServer1UDPRead
    Left = 1323
    Top = 1024
  end
end
