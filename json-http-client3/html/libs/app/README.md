# App.js

A simple javascript-framework for building desktop-like applications.

[http://github.com/summerstyle/app.js](http://github.com/summerstyle/app.js)

Released under the MIT License

## Components

### constructor `App.Window`

Move a block content to window and apply javascript-module for it.

```javascript
var code = new App.Window({
    content_el : utils.dom.id('code'),
    overlay : true,
    js_module : function(self) {
        return {
            print: function() {
                self.el.innerHTML = selected_keys.generate_config();
                self.show();
            }
        };
    }
});
```
Parameters:

* content_el* – a dom-element for including in window,
* overlay – a window with overlay,
* closable – a window can be closed,
* layout – a style of window,
* js_module – a function returns an object with methods, which are mixed in a standard window methods.

The window interface:

* show() — show window,
* hide() — hide window.

### constructor `App.Menu`

This conctructor creates multilevel menu.

The menu constructor called with two arguments: dom-element with link lists and an object with name-action mapping.

```html
<nav id="nav" class="clearfix">
    <ul class="menu menu_level1">
        <li data-action="get_config" class="menu__item">
            <span class="menu__item-name">get config</span>
        </li>
        <li data-action="settings" class="menu__item">
            <span class="menu__item-name">settings</span>
        </li>
        <li data-action="clear" class="menu__item">
            <span class="menu__item-name">clear</span>
        </li>
        <li data-action="show_help" class="menu__item">
            <span class="menu__item-name">?</span>
        </li>
    </ul>
</nav>
```

```javascript
var menu = new App.Menu(utils.dom.id('nav'), {
    'clear' : function() {
        // Deselect all keys
        if (confirm('Clear all?')) {
            selected_keys.clear();
            code.hide();
        };
    },
    'get_config' : function() {
        code.print();
    },
    'show_help' : function() {
        help.show();
    },
    'settings' : function() {
        settings_form.show();
    }
});
```
