from math import *
import numpy as np

def Forward_Kinematic(q1, q2, q3):
    q1 = float(q1)
    q2 = float(q2)
    q3 = float(q3)
    # Khai báo biến
    l1 = 105
    l2 = 162
    l3 = 130
    d1 = 245
    d3 = 55

    # Chuyển đổi góc từ độ sang radian
    q1 = np.radians(q1)
    q2 = np.radians(q2)
    q3 = np.radians(q3)

    # Tính toán px, py, pz
    Px = (l1 * np.cos(q1) - d3 * np.sin(q1) 
          + l2 * np.cos(q1) * np.cos(q2) 
          + l3 * np.cos(q1) * np.cos(q2) * np.cos(q3) 
          - l3 * np.cos(q1) * np.sin(q2) * np.sin(q3))

    Py = (l1 * np.sin(q1) + d3 * np.cos(q1) 
          + l2 * np.cos(q2) * np.sin(q1) 
          + l3 * np.cos(q2) * np.cos(q3) * np.sin(q1) 
          - l3 * np.sin(q1) * np.sin(q2) * np.sin(q3))

    Pz = (d1 + l2 * np.sin(q2) 
          + l3 * np.cos(q2) * np.sin(q3) 
          + l3 * np.cos(q3) * np.sin(q2))

    return Px, Py, Pz

def normalize_angle(angle):
    # Điều chỉnh góc vào khoảng -180 đến 180 độ
    angle = (angle + 180) % 360 - 180
    return angle

def Inverse_Kinematic(Px, Py, Pz):
    # Khai báo biến
    l1 = 114.5
    l2 = 162
    l3 = 130
    d1 = 250
    d3 = 55
    
    a = Py
    b = -Px
    c = d3

    # Tính giá trị q1
    q11 = 2 * np.arctan((b + np.sqrt(b**2 + a**2 - c**2)) / (a + c))
    q12 = 2 * np.arctan((b - np.sqrt(b**2 + a**2 - c**2)) / (a + c))

    Qx1 = Px + d3 * np.sin(q11)
    Qx2 = Px + d3 * np.sin(q12)
    Qy1 = Py - d3 * np.cos(q11)
    Qy2 = Py - d3 * np.cos(q12)

    E1 = Qx1 * np.cos(q11) + Qy1 * np.sin(q11) - l1
    E2 = Qx2 * np.cos(q12) + Qy2 * np.sin(q12) - l1
    Iz = Pz - d1
    F1 = E1**2 + Iz**2 - l2**2 - l3**2

    c31 = F1 / (2 * l2 * l3)
    s31 = np.sqrt(1 - c31**2)
    s32 = -np.sqrt(1 - c31**2)

    q31 = np.arctan2(s31, c31)
    q32 = np.arctan2(s32, c31)

    # Tính Theta2
    TS_c21 = E1 * (l2 + l3 * c31) + l3 * s31 * Iz
    MS_c21 = (l2 + l3 * c31)**2 + (l3 * s31)**2

    TS_c22 = E1 * (l2 + l3 * c31) + l3 * s32 * Iz
    MS_c22 = (l2 + l3 * c31)**2 + (l3 * s32)**2

    TS_s21 = Iz * (l2 + l3 * c31) - l3 * s31 * E1
    MS_s21 = (l2 + l3 * c31)**2 + (l3 * s31)**2

    TS_s22 = Iz * (l2 + l3 * c31) - l3 * s32 * E1
    MS_s22 = (l2 + l3 * c31)**2 + (l3 * s32)**2

    c21 = TS_c21 / MS_c21
    c22 = TS_c22 / MS_c22
    s21 = TS_s21 / MS_s21
    s22 = TS_s22 / MS_s22

    q21 = np.arctan2(s21, c21)
    q22 = np.arctan2(s22, c22)
    
    q11_do = normalize_angle(np.degrees(q11))
    q12_do = normalize_angle(np.degrees(q12))
    q21_do = normalize_angle(np.degrees(q21))
    q22_do = normalize_angle(np.degrees(q22))
    q31_do = normalize_angle(np.degrees(q31))
    q32_do = normalize_angle(np.degrees(q32))
    
    return q11_do, q22_do, q32_do