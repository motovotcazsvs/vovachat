(function() {
    console.log("[JS] ChatGPT script initialized. Awaiting QWebChannel.");

    // Функція, яка запускається після ініціалізації QWebChannel
    function connectToQt() {
        if (typeof qt === 'undefined' || !qt.webChannel) {
            console.error("Qt WebChannel API not found.");
            // Спробувати підключитися ще раз через секунду, якщо не готово
            setTimeout(connectToQt, 1000);
            return;
        }

        new qt.webChannel(function(channel) {
            // Отримуємо доступ до нашого C++ об'єкта, який ми назвали "qtBackend" в main.cpp
            window.backend = channel.objects.qtBackend;

            if (window.backend) {
                console.log("[JS] QWebChannel connected. Backend available.");

                // Встановлюємо функцію для надсилання повідомлень з QML/C++ до TextArea
                window.backend.sendMessage.connect(function(text) {
                    console.log("[JS] Received message from C++:", text);
                    // Викликаємо функцію для емуляції введення тексту та натискання Enter
                    simulateInputAndSend(text.replace("[SEND] ", ""));
                });

                // Спостерігач за DOM, який викликає C++ функцію
                setupDOMObserver();
            } else {
                console.error("[JS] qtBackend object not found in the channel.");
            }
        });
    }

    // Функція симуляції введення тексту (залишається майже без змін)
    function simulateInputAndSend(text) {
        const textarea = document.querySelector("textarea");
        if (!textarea) {
            console.log("No textarea found");
            return;
        }
        textarea.value = text;
        textarea.dispatchEvent(new Event("input", { bubbles: true }));
        textarea.dispatchEvent(new KeyboardEvent("keydown", { key: "Enter", bubbles: true }));
    }

    // Функція спостереження за відповіддю асистента
    function setupDOMObserver() {
        const observer = new MutationObserver(() => {
            // NOTE: Це дуже спрощений спостерігач. У реальному світі ChatGPT DOM змінюється постійно.
            // Краще спостерігати за появою нової кнопки "Stop generating".
            let msgs = document.querySelectorAll('div[data-message-author-role="assistant"]');
            if (msgs.length === 0) return;

            let last = msgs[msgs.length - 1];
            let txt = last.innerText;

            // Надсилаємо відповідь назад у C++ через викликаний метод
            if (window.backend && window.backend.onResponseFromJS) {
                window.backend.onResponseFromJS(txt);
            }
        });
        observer.observe(document.body, { childList: true, subtree: true });
    }

    // Запускаємо процес підключення
    connectToQt();
})();
