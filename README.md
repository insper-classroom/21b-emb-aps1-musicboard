# 21a-emb-aps1

Embarcados - APS 1 - Musical!

Para maiores informações acesse:

https://insper.github.io/ComputacaoEmbarcada/APS-1-Musical/

Issues serão abertos neste repositório para guiar o desenvolvimento
da dupla. **Vocês não devem fechar os issues, apenas a equipe de professores!**, porém devem referenciar nos commits quando um issue 
foi concluído! Isso gerará um PR no classroom que será avaliado pela equipe.

## Documentação

| Função  | PINO (ex: PA11) |
|---------|-----------------|
| BUZZER  |       PD30      |
| START   |       PD28      |
| SELEÇÃO |       PC31      |

### Imagem da montagem

![Montagem](https://github.com/insper-classroom/21b-emb-aps1-musicboard/blob/main/img/esquema.jpeg)

### Vídeo do projeto

[![Assista o vídeo](https://img.youtube.com/vi/MqwybRhX_70/maxresdefault.jpg)](https://youtu.be/MqwybRhX_70)
https://youtu.be/MqwybRhX_70

### Como usar o projeto
- Instale o [Microchip Studio](https://www.microchip.com/en-us/development-tools-tools-and-software/microchip-studio-for-avr-and-sam-devices#Downloads)
- Configure a protoboard com o buzzer e o OLED de acordo com a imagem da montagem acima
- Abra o arquivo *firmware/OLED1.cproj* com o Microchip Studio
- Clique em **Start Without Debugging** na barra de tarefas (Ctrl + Alt + F5)
- Botão 1 -> Play/Pause
- Botão 2 -> Troca de música

### Músicas e Direitos autorais

- Hedwig's Theme (© 2001 WARNER BROS-BARHAM MUSIC, LLC). Disponível em: https://github.com/robsoncouto/arduino-songs/blob/master/harrypotter/harrypotter.ino.
- Cantina Band (© 1977 SONY MUSIC ENTERTAINMENT, LLC). Disponível em: https://github.com/robsoncouto/arduino-songs/blob/master/starwars/starwars.ino.
