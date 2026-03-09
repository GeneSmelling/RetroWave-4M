## Sound Commands

### Text-to-Speech (TTS)

TTS text$ [, lang$ [, mode$]];
Normative queue semantics: FIFO, no overlap, no implicit suppression.

Control commands:
- SAYSTOP
- SAYFLUSH
- SAYSTATUS()
- SAYQUEUE()

Examples:
10. SAY ‘Hello, World!’;
20. TTS ‘Good Morning’, ‘en’;  
30. TTS ‘This is a test of the TTS system.’;