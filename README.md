# FCW-TTC System using HC-SR04

Raspberry Pi 기반 Linux 환경에서 HC-SR04 초음파 센서를 이용하여 구현한 FCW(Forward Collision Warning) 시스템입니다.  
거리 측정, TTC 계산, LED 및 Buzzer 경고 제어를 멀티스레드 구조로 분리하여 실시간 위험 판단 시스템을 구현하였습니다.

## Project Overview

본 프로젝트는 전방 장애물과의 거리를 초음파 센서로 측정하고, 거리 변화량을 기반으로 TTC(Time To Collision)를 계산하여 충돌 위험 단계를 판단하는 시스템입니다.

Linux 환경에서 센서 측정, 판단 로직, 출력 제어를 기능별로 분리하여 멀티스레드 기반으로 동작하도록 구성했습니다.

## Main Features

- HC-SR04 초음파 센서 기반 거리 측정
- 거리 변화량 기반 상대 속도 계산
- TTC 기반 위험 단계 판단
- 위험 단계별 LED 출력
- 위험 단계별 Buzzer 경고
- Linux 기반 멀티스레드 구조
- 센서, 판단 로직, 출력 제어 모듈화

## System Structure

- sensors/
  - HC-SR04 거리 측정 관련 코드

- logic/
  - TTC 계산 및 위험 단계 판단 로직

- output/
  - LED 및 Buzzer 출력 제어 코드

- main.c
  - 전체 스레드 생성 및 시스템 실행 흐름 관리

- Makefile
  - 빌드 설정

## Risk Level

- SAFE
  - 충돌 위험이 낮은 상태
  - 경고 출력 없음 또는 일반 상태 표시

- WARNING
  - 충돌 가능성이 증가한 상태
  - LED 및 Buzzer 경고 출력

- DANGER
  - 즉각적인 충돌 위험이 높은 상태
  - 강한 경고 출력

## Tech Stack

- C
- Raspberry Pi
- Linux
- HC-SR04
- GPIO
- POSIX Thread
- Makefile
- LED
- Buzzer

## Key Implementation Points

- 초음파 센서를 이용한 실시간 거리 측정
- 이전 거리값과 현재 거리값을 이용한 거리 변화량 계산
- TTC 기반 위험 상태 판단
- 멀티스레드 구조를 통한 센서 입력과 출력 제어 분리
- 기능별 파일 분리를 통한 코드 구조화

## Build & Run

Raspberry Pi Linux 환경에서 실행합니다.

1. GPIO 핀 연결
2. 프로젝트 빌드
3. 실행 파일 실행

주의: GPIO 핀 번호와 HC-SR04 연결 방식은 실제 하드웨어 연결에 맞게 코드에서 확인 및 수정해야 합니다.

## Result

본 프로젝트를 통해 Linux 기반 임베디드 환경에서 센서 입력, 위험 판단 알고리즘, 출력 제어가 연결되는 FCW 시스템을 구현하였습니다.  
특히 멀티스레드 구조를 적용하여 거리 측정과 경고 출력이 병렬적으로 수행되도록 구성했습니다.
