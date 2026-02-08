(function () {
    console.log("[JS] ChatGPT script initialized. Awaiting QWebChannel.");

    function connectToQt() {
        if (typeof qt === 'undefined' || !qt.webChannel) {
            setTimeout(connectToQt, 500);
            return;
        }

        new qt.webChannel(window.qt.webChannelTransport, function (channel) {
            console.log("[JS] WebChannel initialized. Available objects:", Object.keys(channel.objects));
            window.backend = channel.objects.qtBackend;

            if (window.backend) {
                console.log("[JS] QWebChannel connected. Backend available.");
                setupDOMObserver();
            } else {
                console.error("[JS] qtBackend object not found in the channel. Objects present:", Object.keys(channel.objects));
            }
        });
    }

    function setupDOMObserver() {
        let lastSentText = "";

        const observer = new MutationObserver(() => {
            // Шукаємо останнє повідомлення асистента
            let msgs = document.querySelectorAll('div[data-message-author-role="assistant"]');
            if (msgs.length === 0) return;

            let lastMsg = msgs[msgs.length - 1];

            // Перевіряємо, чи завершилася генерація. 
            // Зазвичай, коли ChatGPT закінчує, з'являються кнопки дій (Copy, Good/Bad response)
            // Або зникає кнопка "Stop generating"
            let isGenerating = !!document.querySelector('button[aria-label="Stop generating"]');

            if (!isGenerating) {
                let txt = lastMsg.innerText.trim();
                // Відправляємо тільки якщо текст змінився і він не порожній
                if (txt && txt !== lastSentText) {
                    lastSentText = txt;
                    console.log("[JS] ChatGPT reply complete:", txt);
                    if (window.backend && window.backend.onResponseFromJS) {
                        window.backend.onResponseFromJS(txt);
                    }
                }
            }
        });

        observer.observe(document.body, { childList: true, subtree: true });
    }

    connectToQt();
})();
