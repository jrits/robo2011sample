ET���{�R��2010�pNXT���s��(NXTway-ET) C++�T���v���v���O����

					             2010/02 ET���{�R���Z�p�ψ���
=================================================================================

1.�͂��߂�
�{�v���O������ET���{�R��2010��NXT���s��(NXTway-ET)�̃T���v��C++�v���O�����ł��B
�T���v���v���O���������s����ꍇ�́Aetrobo2010sample(nxtOSEK)�t�H���_��nxtOSEK�Ɠ����f�B���N�g���K�w
�ɔz�u���Ă��������B(��AC:/cygwin/nxtOSEK�Ȃ��C:/cygwin/etrobo2010sample(nxtOSEK))
�{�v���O�����̎g�p�ɂ�nxtOSEK v2.11�ȍ~�̃o�[�W�������K�v�ɂȂ�܂��B

2.�t�@�C���\��
etrobo2010sample(nxtOSEK)/sample_cpp�t�H���_���͈ȉ��̃t�@�C������\������Ă��܂��B
 balancer_param.c            �o�����X����p�����[�^C�\�[�X�t�@�C��
 Driver.h                    ���s�̃h���C�o�[�x�[�X�N���X�w�b�_�t�@�C��
 DriverManager.cpp           ���s�̃h���C�o�[�Ǘ��N���X�\�[�X�t�@�C��
 DriverManager.h             ���s�̃h���C�o�[�Ǘ��N���X�w�b�_�t�@�C��
 LineTraceDriver.cpp         ���C���g���[�X���s�h���C�o�[�N���X�\�[�X�t�@�C��
 LineTraceDriver.h           ���C���g���[�X���s�h���C�o�[�N���X�w�b�_�t�@�C��
 NXTway_GS.cpp               ���s�̃N���X�\�[�X�t�@�C��
 NXTway_GS.h                 ���s�̃N���X�w�b�_�t�@�C��
 NXTway_GS_data.h            ���s�̓����f�[�^�w�b�_�t�@�C��
 Main.cpp   	             TOPPERS/ATK1�^�X�N��`�\�[�X�t�@�C��
 Main.oil                    TOPPERS/ATK1�ݒ�t�@�C��
 SonarDriver.cpp             �����g�Z���T��Q�����o�h���C�o�[�\�[�X�t�@�C��
 SonarDriver.h               �����g�Z���T��Q�����o�w�b�_�\�[�X�t�@�C��
 StartStopDriver.cpp         ���s�̃X�^�[�g/�X�g�b�v����h���C�o�[�\�[�X�t�@�C��
 StartStopDriver.h           ���s�̃X�^�[�g/�X�g�b�v����h���C�o�[�w�b�_�t�@�C��
 Makefile                    �v���O�����r���h�pMakefile
 README.txt
 doxygen.cfg                 doxygen�ݒ�t�@�C��
 /html/index.html            doxygen�����N���XAPI���t�@�����X

3. �r���h���@
- nxtOSEK v2.11�ȍ~�̊J�����\�z�������Ȃ��܂��B
- Cygwin���sample_cpp�f�B���N�g���Ɉړ����A
   $ make all
 �Ɠ��͂��܂��B�T���v���v���O������TOPPERS/ATK1�ƃ����N����A���s�`���t�@�C������������܂��B
- nxtOSEK�̑�����@�ɏ]���āA�T���v���v���O������NXT�ɃA�b�v���[�h���܂��B

4. �T���v���v���O�����̑�����@
- �T���v���v���O�������N�����܂��B������ʂ̕\����A���C����ʂ��\������܂��B
- RUN�{�^��(�E���̎O�p�{�^��)�������܂��B
- NXTway-ET�𗧂�������ԂŁA�����v���ɍ��������G�b�W��ɔz�u���܂��B
  ���̎��A���E�ԗւ͕K���n�ʂɐڒn�����Ă��������B
- NXTway-ET�̉E���ɑ�������Ă���^�b�`�Z���T���������ƂŁA�W���C���Z���T�̎����L�����u���[�V�����������Ȃ��A
  �u�U�[���Ȃ�����A���C���g���[�X���J�n���܂��B���s���Ƀ^�b�`�Z���T���������ƂŁA���s���~���܂��B
  �^�b�`�Z���TON�̃g�O������ɂ��A���s�J�n/���s��~�𑀍�ł��܂��B
���W���C���Z���T�̎����L�����u���[�V�������́A�p���x0[deg/sec]���̃W���C���Z���T�I�t�Z�b�g�l���v�Z���Ă��邽�߁A
  �u�U�[���Ȃ�܂ŁA���{�b�g�𓮂����Ȃ��ł��������B
- ���s�̑O����30cm�ȓ��ɏ�Q��������ꍇ�A�����g�Z���T���o�ɂ��A���C���g���[�X���s���~���܂��B
�������g�Z���T�̋������茋�ʂ́A���͋C���x���Q���̌`��E�ގ��ɂ��قȂ�܂��B�܂��A��Q����
  �����g�Z���T�ɋߐڂ��Ă���ꍇ(��, 5cm�ȓ�)�́A�듮�삷��ꍇ������܂��B

5. ���l
�� ET���{�R���݌v�E�������� �z�z�v���O�����̎�舵���ɂ���
�z�z�v���O�����́A�ʏ�̉^�p�ɂ����Ă͉�����̂Ȃ����Ƃ��m�F���Ă���܂����A
�^�p���ʁA�����Ȃ鑹�Q�����������Ƃ��Ă�����҂Ȃ�т�ET�\�t�g�E�F�A�f�U�C�����{�R��
�Z�p�ψ���͂����Ȃ�ӔC�������܂���B�g�p�҂̐ӔC�ɂ����Ă����p���������B

�� ������ɂ���
�z�z�v���O�����̎g�����ɂ��Ă̂�����͂��f��\���グ�܂��B���������������B

