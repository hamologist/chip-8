var displayScale = 16,
    standardX = 64,
    standardY = 32,
    fps = 1000 / 60,
    newGameSelected = false,
    game = "games/pong2.c8",
    onColor = "#FFFFFF",
    offColor = "#000000"
    lastUsedKey = null,
    keys = {
      "Digit1": 0x1, "Digit2": 0x2, "Digit3": 0x3, "Digit4": 0xC,
      "KeyQ":   0x4, "KeyW":   0x5, "KeyE":   0x6, "KeyR":   0xD,
      "KeyA":   0x7, "KeyS":   0x8, "KeyD":   0x9, "KeyF":   0xE,
      "KeyZ":   0xA, "KeyX":   0x0, "KeyC":   0xB, "KeyV":   0xF,
    };

var Module = {
  onRuntimeInitialized: function() {
    var cpu = new Module.ChipEight(),
        ctx = document.getElementById("chipEightDisplay").getContext("2d"),
        renderFrame = function (displayBuffer, scale) {
          var x = 64,
              y;
          ctx.fillStyle = offColor;
          ctx.fillRect(0, 0, 64 * scale, 32 * scale);
          ctx.fillStyle = onColor;
          while (x >= 0) {
            y = 32;
            while (y >= 0) {
              if (displayBuffer[y * 64 + x] == 1) {
                ctx.fillRect(x * scale, y * scale, scale, scale);
              }
              y--;
            }
            x--;
          }
        },
        processInput = function () {
          var i = 15,
              handleLastUsedKey = function () {
                while (i >= 0) {
                  cpu.set_key(i, 0);
                  i--;
                }
                if (lastUsedKey !== null) {
                  cpu.set_key(lastUsedKey, 1)
                }
              };
          if (cpu.get_is_waiting()) {
            if (lastUsedKey === null) {
              return false;
            }
            else {
              cpu.set_is_waiting(false)
              handleLastUsedKey();
            }
          }
          else {
              handleLastUsedKey();
          }
          return true;
        },
        main;

    cpu.load_file(game);

    main = function () {
      if (processInput()) {
        cpu.execute_instruction();
        renderFrame(cpu.get_display_memory(), displayScale);
      }
      if (newGameSelected) {
        cpu.delete();
        cpu = new Module.ChipEight();
        cpu.load_file(game);
        newGameSelected = false;
      }
      setTimeout(main, fps);
    };
    window.setTimeout(main, fps);
  }
};

window.onload = function () {
  var chipEightCanvas = document.getElementById("chipEightDisplay"),
      chipEightScale = document.getElementById("chipEightScale"),
      chipEightFPS = document.getElementById("chipEightFPS"),
      chipEightOnColor = document.getElementById("chipEightOnColor"),
      chipEightOffColor = document.getElementById("chipEightOffColor"),
      chipEightGamePicker = document.getElementById("chipEightGamePicker"),
      lastDownTarget;

  chipEightScale.addEventListener("change", function() {
    displayScale = parseInt(chipEightScale.value)
    chipEightCanvas.setAttribute("width", standardX * displayScale);
    chipEightCanvas.setAttribute("height", standardY * displayScale);
  });

  chipEightFPS.addEventListener("change", function() {
    fps = 1000 / parseInt(chipEightFPS.value)
  });

  chipEightGamePicker.addEventListener("change", function() {
    game = chipEightGamePicker.value;
    newGameSelected = true;
  });

  chipEightOnColor.addEventListener("keypress", function (e) {
    var key = e.which || e.keyCode;
    if (key === 13) {
      onColor = chipEightOnColor.value;
    }
  });

  chipEightOffColor.addEventListener("keypress", function (e) {
    var key = e.which || e.keyCode;
    if (key === 13) {
      offColor = chipEightOffColor.value;
    }
  });

  document.addEventListener("mousedown", function (e) {
    lastDownTarget = e.target;
  }, false);

  document.addEventListener("keydown", function (e) {
    if (lastDownTarget != chipEightCanvas) {
      return;
    }

    if (e.preventDefaulted) {
      return;
    }

    if (keys[e.code] !== undefined) {
      lastUsedKey = keys[e.code];
    }

    e.preventDefault();
  }, false);

  document.addEventListener("keyup", function(e) {
    if (lastDownTarget != chipEightCanvas) {
      return;
    }

    if (e.preventDefaulted) {
      return;
    }

    if (keys[e.code] !== undefined) {
      lastUsedKey = null;
    }

    e.preventDefault();
  });
}
