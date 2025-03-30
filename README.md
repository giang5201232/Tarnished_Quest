# **Tarnished Quest - Project Documentation**  

## **1. Thông tin về game**  
### **🔗 Link**  
- **GitHub Repository**: [https://github.com/yourname/TarnishedQuest](https://github.com/yourname/TarnishedQuest)  
- **Gameplay Demo (YouTube)**: [https://youtu.be/demo](https://youtu.be/demo) *(Nếu có)*  

### **🎮 Game tham khảo**  
| Game | Link | Mô tả |  
|------|------|-------|  
| **ULTRAKILL** | [Steam](https://store.steampowered.com/app/1229490/ULTRAKILL/) | Cơ chế **Style Meter** thúc đẩy lối chơi tấn công liên tục |  
| **One Finger Death Punch** | [Steam](https://store.steampowered.com/app/264200/One_Finger_Death_Punch/) | Beat 'em up tốc độ cao, yêu cầu phản xạ nhanh |  
| **Hotline Miami** | [Steam](https://store.steampowered.com/app/219150/Hotline_Miami/) | Áp lực thời gian, 1-hit kill, gameplay tàn khốc |  

---

## **2. Các tính năng cải tiến trong game**  
✔ **Hệ thống "Soul Hunger"**  
- Người chơi **phải liên tục giết địch** để duy trì thanh năng lượng, nếu không sẽ **mất máu dần**.  
- Lấy cảm hứng từ **ULTRAKILL**, nhưng thay vì chỉ giảm điểm, game này **trừng phạt trực tiếp bằng mất máu**.  

✔ **Level ngẫu nhiên vô tận**  
- Sinh map từ **15 bản thiết kế sẵn**, không lặp lại trình tự.  
- Độ khó tăng dần theo thời gian (nhiều quái hơn, địch mạnh hơn).  

✔ **AI quái vật thông minh**  
- Skeleton **tự động di chuyển**, **đuổi theo người chơi** khi phát hiện.  
- Có **tầm tấn công** và **knockback** để tăng tính thách thức.  

✔ **Animation & Âm thanh chi tiết**  
- 5+ trạng thái animation (chạy, nhảy, tấn công, bị đánh, chết).  
- Nhạc nền điện tử + hiệu ứng âm thanh **bắn/giết/nhảy** chân thực.  

---

## **3. Cấu trúc của project game**  
### **📂 Tổ chức lớp (Class Structure)**  
```bash
📁 TarnishedQuest/  
├── 📄 main.cpp            # Khởi tạo game, vòng lặp chính  
├── 📁 src/  
│   ├── 📄 Player.h/cpp    # Xử lý di chuyển, tấn công, máu  
│   ├── 📄 Skeleton.h/cpp  # AI quái vật, hành vi tấn công  
│   ├── 📄 LevelPart.h/cpp # Quản lý map ngẫu nhiên  
│   ├── 📄 Game.h/cpp      # Logic chính (render, update, input)  
│   └── 📄 Menu.h/cpp      # Giao diện menu/pause  
├── 📁 res/                # Tài nguyên  
│   ├── 📁 texture/        # Hình ảnh (player, quái, map)  
│   ├── 📁 sfx/            # Âm thanh (nhạc, hiệu ứng)  
│   └── 📁 font/           # Font chữ  
└── 📄 README.md           # Hướng dẫn cài đặt  
```

### **🔄 Luồng hoạt động (Game Loop)**  
1. **Khởi tạo** (SDL, texture, âm thanh, map).  
2. **Menu chính** → Chọn **Bắt đầu/Tùy chọn/Thoát**.  
3. **Vào game**:  
   - Load map ngẫu nhiên.  
   - Spawn player và quái vật.  
4. **Gameplay Loop**:  
   - Người chơi di chuyển/tấn công.  
   - Quái vật đuổi theo và tấn công.  
   - **Nếu không giết địch kịp thời → mất máu**.  
5. **Kết thúc**:  
   - Khi người chơi chết → Hiện điểm + nút **Chơi lại**.  

---

## **4. Nguồn tham khảo và dữ liệu**  
### **🎨 Đồ họa**  
- **Player/Enemy Sprites**: Tự vẽ bằng Aseprite/Pixel Art.  
- **Tilemap**: Sử dụng từ [OpenGameArt.org](https://opengameart.org/).  

### **🎵 Âm thanh**  
| File | Nguồn |  
|------|-------|  
| Nhạc nền | [FreeMusicArchive](https://freemusicarchive.org/) |  
| SFX (bắn, nhảy) | [Freesound](https://freesound.org/) |  

### **📚 Code & Hướng dẫn**  
- **SDL Tutorials**: [LazyFoo.net](https://lazyfoo.net/tutorials/SDL/index.php)  
- **AI Movement**: Thuật toán Dijkstra đơn giản cho quái vật.  
- **ChatGPT**: Hỗ trợ debug logic game.  

---

## **5. Thông tin khác**  
### **⚠️ Lưu ý khi chạy game**  
- Cần cài đặt **SDL2, SDL2_image, SDL2_mixer, SDL2_ttf**.  
- Game chỉ chạy trên **Windows/Linux** (chưa hỗ trợ Mac).  

### **📅 Kế hoạch phát triển**  
- [ ] Thêm **boss fight** cuối mỗi 10 level.  
- [ ] Chế độ **"Hardcore"** (1 hit chết).  
- [ ] Multiplayer (PvP hoặc Co-op).  

**🎯 Mục tiêu:** Tạo ra một game **khó nhưng công bằng**, buộc người chơi phải **tối ưu từng giây**.  

> *"Bạn không chạy trốn. Bạn không trốn tránh. Bạn GIẾT."* 🔥
