# memo

- add KEYS declaration to appinfo.json
- http://www.dafont.com/ search font at dafont

## app menu incoming
http://developer.getpebble.com/guides/pebble-apps/resources/image-resources/#setting-the-app-39-s-menu-icon

menu icon must be 2-color indexed png image

```
"resources": {
  "media": [
    {
      "menuIcon": true,
      "type": "png",
      "name": "IMAGE_MENU_ICON",
      "file": "images/menu_icon.png"
    }
  ]
}
```
