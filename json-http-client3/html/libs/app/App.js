/*
 * App.js
 * http://github.com/summerstyle/app.js
 *
 * Copyright 2015 Vera Lobacheva (summerstyle.ru)
 * Released under the GPL3 (GPL3.txt)
 */

'use strict';

/**
 * The application
 * @returns {Object} The application interface with Menu and Window constructors
 */
var App = (function() {
    /* Utilities */
    var utils = {
        dom : {
            css : function(el, prop, value) {
                el.style[prop] = value;
            },
            show : function(el) {
                this.css(el, 'display', 'block');
            },
            hide : function(el) {
                this.css(el, 'display', 'none');
            },
            id : function(id) {
                return document.getElementById(id);
            },
            klass : function(klass) {
                return document.getElementsByClassName(klass);
            },
            stopEvent : function(e) {
                e.stopPropagation();
                e.preventDefault();
                
                return this;
            },
            matches : (function() {
				var el = document.createElement('div'),
					matches = el.matches ||
                              el.mozMatchesSelector ||
                              el.webkitMatchesSelector ||
                              el.oMatchesSelector ||
                              el.msMatchesSelector;
				
				return function(selector, el) {
					if (!matches) {
						return false;
					}
					
					return matches.call(el, selector);
				}
			})(),
            closest : function(el, selector) {
				var parent = el,
					result = false;
				
				while (parent && parent !== document) {
					if (this.matches(selector, parent)) {
						result = parent;
						break;
					}
					
					parent = parent.parentNode;
				}
				
				return result;
			}
        },
        extend : function(obj, options) {
            var target = {};
            
            for (name in obj) {
                if(obj.hasOwnProperty(name)) {
                    target[name] = options[name] ? options[name] : obj[name];
                };
            };
            
            return target;
        },
        mixin : function(target, mixin) {
            for (var prop in mixin) {
                if (mixin.hasOwnProperty(prop)) {
                    target[prop] = mixin[prop];
                }
            }
        },
        foreach : function(arr, func) {
            for(var i = 0, count = arr.length; i < count; i++) {
                func(arr[i], i);
            };
        },
        foreachReverse : function(arr, func) {
            for(var i = arr.length - 1; i >= 0; i--) {
                func(arr[i], i);
            };
        },
        inherits : (function() {
            var F = function() {};
            
            return function(Child, Parent) {
                F.prototype = Parent.prototype;
                Child.prototype = new F();
                Child.prototype.constructor = Child;
            }
        })(),
        debug : (function() {
            var output = document.getElementById('debug');
            
            return function() {
                output.innerHTML = [].join.call(arguments, ' ');
            };
        })(),
    };
    
    /**
     * Creates a new menu
     * @constructor
     * @param {HTMLElement} el     - The root element of menu
     * @param {object}      config - The dict with name-action mapping
     */
    var Menu = function(el, config) {
        el.addEventListener('click', function(e) {
            var menu_item = utils.dom.closest(e.target, '.menu__item'),
                action = menu_item ? menu_item.dataset['action'] : null;
            
            if (action && action in config) {
                config[action]();    
            }
        }, false);    
    };
    
    
    /**
     * Creates a new app window
     * @constructor
     * @param {object} config - The params of this window
     */
    function Window(config) {
        var defaults = {
            closable: true,
            overlay: true,
            layout: 'default',
            content_el: null,
            js_module: null
        };
        
        var params = utils.extend(defaults, config);
        var MIN_Z_INDEX = 10;
        
        this.content_el = params.content_el;
        
        var window_el = document.createElement('div'),
            content_wrapper = document.createElement('div');
            
        window_el.className = 'window';
        content_wrapper.className = 'window__content';
        
        if (params.overlay) {
            var overlay = Window.create_overlay();
        }
        
        if (params.content_el.dataset['header']) {
            var header = document.createElement('h4');
            header.className = 'window__header';
            header.innerHTML = params.content_el.dataset['header'];
            window_el.appendChild(header);
        }
        
        window_el.appendChild(content_wrapper);
        document.body.appendChild(window_el);
        
        // Add content 
        if (params.content_el) {
            content_wrapper.appendChild(params.content_el);
        }

        // Add close button + events
        if (params.closable) {
            var close_button = document.createElement('div');
            close_button.className = 'window__close-button';
            window_el.appendChild(close_button);
            
            close_button.addEventListener('click', hide, false);
            overlay.addEventListener('click', hide, false);
        }
 
        function show() {
            utils.dom.show(window_el);
            
            if (overlay) {
                utils.dom.show(overlay);
            }
            
            Window.layers.push(this);
        }
        
        function hide() {
            utils.dom.hide(window_el);
            
            if (overlay) {
                utils.dom.hide(overlay);
            }
            
            Window.layers.pop(this);
        }
        
        this.show = show;
        this.hide = hide;
        this.set_layer = function(n) {
            utils.dom.css(window_el, 'zIndex', MIN_Z_INDEX + n * 2 + 1);
            if (params.overlay) {
                utils.dom.css(overlay, 'zIndex', MIN_Z_INDEX + n * 2);
            }
        };
        
        // JS module
        if (typeof params.js_module === 'function') {
            var result = params.js_module.call(this, this);
            utils.mixin(this, result);
        }
    }
    
    Window.layers = (function() {
        var arr = [],
            current = 0;
        
        return {
            push : function(win) {
                win.set_layer(++current);
                arr.push(win);
            },
            pop : function(win) {
                if (win) {
                    arr.splice(arr.indexOf(win), 1);
                } else {
                    arr.pop();
                }
                
                current--;
            },
            clear : function() {
                while(arr.length) {
                    this.pop();
                }
            }
        };
    })();
    
    /**
     * @function create_overlay
     * @memberof Window
     * @static
     * @returns {HTMLElement} - A DOM-element of new overlay
     */
    Window.create_overlay = function() {
        var overlay = document.createElement('div');
        
        overlay.className = 'overlay';
        utils.dom.hide(overlay);
        document.body.appendChild(overlay);
        
        return overlay;
    }
    
    
    return {
        utils : utils,
        Menu : Menu,
        Window : Window
    };
})();
