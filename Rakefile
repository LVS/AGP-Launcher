require "rubygems"
require "rake"

require "choctop"

ChocTop.new do |s|
  # Remote upload target (set host if not same as Info.plist['SUFeedURL'])
  # s.host     = 'agp launcher.com'
  s.remote_dir = '/path/to/upload/root/of/app'

  # Custom DMG
  s.background_file = "Artwork/DMG Background.png"
  s.app_icon_position = [92, 172]
  s.applications_icon_position =  [308, 172]
  s.volume_icon = "Artwork/dmg.icns"
  # s.applications_icon = "appicon.icns" # or "appicon.png"
end
